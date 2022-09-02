#include "pch.h"
#include "Player.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Animator.h"
#include "ColliderBase.h"
#include "NoobRay.h"
#include "Camera.h"
#include "SceneManager.h"
#include "UtilityFunction.h"
#include "Inventory.h"
#include "Item.h"
#include "Panel.h"
#include "Audio.h"
#include "GameProcess.h"
#include "EventMachine.h"
#include "GameManager.h"

Player::Player(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject),
	_transform(gameObject->GetTransform()), _timer(0.0f), _fishingtimer(3.0f),
	rotation(Vector3(0.f, 1.f, 0.f)), _speed(20.f), currentState(PlayerState::Idle), _audio(gameObject->GetComponent<Audio>())
{
	/*_panel = gameObject->GetComponent<Panel>();*/

}

Player::~Player()
{
}

void Player::Awake()
{
	_gameObject = GetGameObject();

	_adultBoy_Idle = hasher("Nod");
	_adultBoy_Row = hasher("Rowing");

	_adultGirl_Idle = hasher("Nod");
	_adultGirl_Row = hasher("Rowing.001");

	targetPoint = _transform->GetWorldPosition();
	_startRoataion = _transform->GetWorldRotation();
}

void Player::FixedUpdate()
{
	_speed = GameManager::GetInstance()->speed;

	if (EventMachine::GetInstance()->isMove)
	{
		if (EventMachine::GetInstance()->isFarming)
			_fishingRange->activeSelf = true;
		else
			_fishingRange->activeSelf = false;

		switch (currentState)
		{
		case PlayerState::Idle:
		{
			if (_isIdleAnimStart)
			{
				_adultBoy->PlayAnim(_adultBoy, _adultBoy_Idle, true);
				_adultGirl->PlayAnim(_adultGirl, _adultGirl_Idle, true);
				_isIdleAnimStart = false;
			}
			_moveCheck = false;

			Idle();
		}
		break;
		case PlayerState::Move:
		{
			if (_isMoveAnimStart)
			{
				_adultBoy->PlayAnim(_adultBoy, _adultBoy_Row, true);
				_adultGirl->PlayAnim(_adultGirl, _adultGirl_Row, true);
				_isMoveAnimStart = false;
			}
			_moveCheck = true;
			Move();
		}
		break;
		case PlayerState::Fishing:
		{
			// TODO : �����Լ� �ۼ� (�����Ŀ� Ÿ�پ����� ��������ϱ�)
			GameManager::GetInstance()->isPlayerStop = true;
			Fishing();
			_moveCheck = false;

		}
		break;
		case PlayerState::Talk:
		{
			Talk();
		}
		break;
		}
		_isIdleAnimCheck = true;
	}
	else
	{
		currentState = PlayerState::Idle;
		_isIdleAnimStart = true;

		if (_isIdleAnimStart && _isIdleAnimCheck)
		{
			_adultBoy->PlayAnim(_adultBoy, _adultBoy_Idle, true);
			_adultGirl->PlayAnim(_adultGirl, _adultGirl_Idle, true);
			_isIdleAnimStart = false;
			_isIdleAnimCheck = false;
		}
		_moveCheck = false;
		if (_isFarmingCancel)
		{
			// fishingGauge UI�� ���ش�.
			_fishingGauge->activeSelf = false;
			_fishingGaugeBack->activeSelf = false;

			// �ı��Ѵ�.
			if (_targetItem != nullptr)
				Destroy(_targetItem);
		}
		Idle();
	}
}

void Player::Update()
{

}

void Player::Idle()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::SPACE))
	{
		if (TargetItem())
		{
			if (!_targetItem->GetScript<Item>()->GetFarmingItem())
			{
				// state�� fishing���� �ٲ��ش�.
				currentState = PlayerState::Fishing;

				// fishingGauge UI�����ش�.
				_fishingGauge->activeSelf = true;
				_fishingGaugeBack->activeSelf = true;
				// �������� �������� �����.
				_targetItem->GetScript<Item>()->SetIsMove(false);
			}
		}
	}

	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		//Screen���� ���� mousePos�� ViewSpace�� �ٲپ [rayOrigin : ���� ��� ��������, direction : ������ ����] �� ray�� return ��
		NoobRay ray = _mainCamera->GetCamera()->ScreenPointToRay(InputManager::GetInstance()->GetMousePos());

		// ray�� �ε����� �ִ��� �Ǵ����ִ� �Լ�
		// �ִٸ� point�� �� ���� World ���·� ����ȴ�.
		if (ray.Intersect())
		{
			targetPoint = ray.point;
			targetPoint.y = _transform->GetWorldPosition().y;		// y�� �״���ؼ� ��������¡~��

			currentState = PlayerState::Move;
			_isMoveAnimStart = true;
		}
	}


	//	ShakeObject(); // �յڷ� �谡 �����̴°�ó�� ������
}

void Player::Move()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::SPACE))
	{
		/*GetGameObject()->GetComponent<Audio>()->PlayEvent("event:/Weapons/Explosion");*/

		if (TargetItem())
		{
			currentState = PlayerState::Fishing;

			// fishingGauge UI�����ش�.
			if (!_targetItem->GetScript<Item>()->GetFarmingItem())
			{
				_fishingGauge->activeSelf = true;
				_fishingGaugeBack->activeSelf = true;
			}
			_adultBoy->PlayAnim(_adultBoy, _adultBoy_Idle, true);
			_adultGirl->PlayAnim(_adultGirl, _adultGirl_Idle, true);

			// �������� �������� �����.
			_targetItem->GetScript<Item>()->SetIsMove(false);
		}
	}

	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		//Screen���� ���� mousePos�� ViewSpace�� �ٲپ [rayOrigin : ���� ��� ��������, direction : ������ ����] �� ray�� return ��
		NoobRay ray = _mainCamera->GetCamera()->ScreenPointToRay(InputManager::GetInstance()->GetMousePos());

		// ray�� �ε����� �ִ��� �Ǵ����ִ� �Լ�
		// �ִٸ� point�� �� ���� World ���·� ����ȴ�.
		if (ray.Intersect())
		{
			targetPoint = ray.point;
			targetPoint.y = _transform->GetWorldPosition().y;		// y�� �״���ؼ� ��������¡~��

			currentState = PlayerState::Move;

			// �����̴� ���߿� �������� targetPoint�� �ٲ� dist�� �ٲ���� �׷���
			// distToPoint�� �ʱ�ȭ ���ش�.
			_distToPoint = 0.f;
		}
	}

	// �ϴ� �̵��ؾ��� �Ÿ��� �����صд�.
	_dist = (_transform->GetWorldPosition() - targetPoint).Length();

	/*if (_dist <= 0.1f)
	{
		currentState = PlayerState::Idle;
		_isIdleAnimStart = true;
	}
	else*/

	if (_distToPoint < _dist)
	{
		GameManager::GetInstance()->isPlayerStop = false;

		Vector3 dir = targetPoint - _transform->GetWorldPosition();

		// �ش� ������ ������ atn2�� ���ϰ� rad�̴� degree�� �ٲٰ� �� �ڽ��� ���ƾ��ϴϱ� 90���� ���ش�.
		/*float angle = XMConvertToDegrees(atan2(dir.z, dir.x));

		if(angle <= 90)
			angle = 90.f - XMConvertToDegrees(atan2(dir.z, dir.x));
		else if(angle > 90 && angle <= 180)
			angle = -(XMConvertToDegrees(atan2(dir.z, dir.x)) - 90.f);
		else if(angle > -180 && angle <= -90)
			angle = -(XMConvertToDegrees(atan2(dir.z, dir.x)) + 270.f);
		else
			angle = -(XMConvertToDegrees(atan2(dir.z, dir.x)) - 90.f);*/

		dir.Normalize();
		//_transform->SetLocalPosition(_transform->GetLocalPosition() + dir * _speed * TimeManager::GetInstance()->GetDeltaTime());

		// ���� �������� �̵��ؾ��� �ӵ� �� �ǳ��ش�.
		GameManager::GetInstance()->SetObjectMove(dir * _speed * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed);

		// ������ �Ÿ��� ���������� ����
		_distToPoint += (dir * _speed * TimeManager::GetInstance()->GetDeltaTime()).Length();

		/*if (angle < 0)
		{
			if (_transform->GetWorldRotation().y > angle)
				_transform->Rotate(Vector3(0, angle * TimeManager::GetInstance()->GetDeltaTime(), 0));
		}
		else
		{
			if(_transform->GetWorldRotation().y <= angle)
				_transform->Rotate(Vector3(0, angle * TimeManager::GetInstance()->GetDeltaTime(), 0));
		}*/

		_transform->LookAt(targetPoint);


	}
	else
	{
		currentState = PlayerState::Idle;
		_isIdleAnimStart = true;

		_distToPoint = 0.f;
		GameManager::GetInstance()->isPlayerStop = true;
	}
}

void Player::ShakeObject()
{
	_frequency += TimeManager::GetInstance()->GetDeltaTime();

	float sinFrequnecy = sinf(_frequency);

	_transform->MoveLook(1.0f * sinFrequnecy * TimeManager::GetInstance()->GetDeltaTime());
}

int Player::ShakingBoat(bool check, bool right)
{
	if (check)
	{
		if (right)
		{
			_transform->Rotate(Vector3(0.f, 0.f, -5.f));
			_rotateCheck += 5;
			return 5;
		}
		else
		{
			_transform->Rotate(Vector3(0.f, 0.f, 5.f));
			_rotateCheck -= 5;
			return -5;
		}
	}
	else
	{
		_rotateCheck = 0;
		return 0;
	}
}

void Player::Talk()
{

}

void Player::Fishing()
{
	if (_isIdleAnimStart)
	{
		_adultBoy->PlayAnim(_adultBoy, _adultBoy_Idle, true);
		_adultGirl->PlayAnim(_adultGirl, _adultGirl_Idle, true);
		_isIdleAnimStart = false;
	}

	//_transform->SetLocalPosition(targetPoint);
	_timer += TimeManager::GetInstance()->GetDeltaTime();

	_fishingGauge->GetComponent<Panel>()->SetOffset(
		GameProcess::_windowInfo.width / 2 - 50.f,
		GameProcess::_windowInfo.width / 2 - 50.f + (_timer * (10000.f / _fishingtimer) / 100.f),
		GameProcess::_windowInfo.height / 2 + 60.f,
		GameProcess::_windowInfo.height / 2 + 80.f);
	_fishingGauge->GetComponent<Panel>()->screenResize = Vector2(1920.f, 1080.f);
	_fishingGauge->GetComponent<Panel>()->Resize(GameProcess::resizeInfo.width, GameProcess::resizeInfo.height);

	if (_timer >= _fishingtimer)
	{
		// UI�������� �ٽ� 0����
		_fishingGauge->GetComponent<Panel>()->SetOffset(
			GameProcess::_windowInfo.width / 2 - 50.f,
			GameProcess::_windowInfo.width / 2 - 50.f,
			GameProcess::_windowInfo.height / 2 + 50.f,
			GameProcess::_windowInfo.height / 2 + 70.f);
		_fishingGauge->GetComponent<Panel>()->screenResize = Vector2(1920.f, 1080.f);
		_fishingGauge->GetComponent<Panel>()->Resize(GameProcess::resizeInfo.width, GameProcess::resizeInfo.height);

		// fishingGauge UI�� ���ش�.
		_fishingGauge->activeSelf = false;
		_fishingGaugeBack->activeSelf = false;

		// ���ӸŴ����� ������ ������ ������ ���������ش�. 
		GameManager::GetInstance()->items[_targetItem->GetScript<Item>()->GetItemType()]++;

		// �ı��Ѵ�.
		if (!_targetItem->GetScript<Item>()->GetFarmingItem())
			Destroy(_targetItem);

		// state Idle�� �ٲ��ش�.
		currentState = PlayerState::Idle;

		_isIdleAnimStart = true;

		_timer = 0;

		_audio->PlayEvent("event:/SFX/SFX_Item2", false);
	}
}

bool Player::ShakingEvent(bool check, bool right)
{
	// �̹�Ʈ�� ����ȴٰ� ������ ��鸰��. 
	if (check)
	{
		_introTotalTime += TimeManager::GetInstance()->GetDeltaTime();

		// 2�ʺ��� ���ٸ� Ű �Է��� �ް��Ѵ�. 
		if (5.0f > _introTotalTime)
		{
			// ���������� ����̰� ������ ���� Ű�Է��� �޾ƾ��Ѵ�. 
			if (right)
			{
				if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::Q))
				{
					_transform->Rotate(Vector3(0.f, 0.f, 10.f));
					_introTotalTime = 0.f;
					return true;
				}
			}
			// �������� ����̰� ������ ������ Ű�Է��� �޾ƾ��Ѵ�. 
			else
			{
				if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::E))
				{
					_transform->Rotate(Vector3(0.f, 0.f, -10.f));
					_introTotalTime = 0.f;
					return true;

				}
			}
		}
		return false;
	}
	else
	{
		{
			// ������� ������ ü���� -5 ���ش�.
			_transform->SetLocalRotation(_startRoataion);

			_introTotalTime = 0.f;
			return false;
		}
	}
}

bool Player::TargetItem()
{
	if (_interactItems.empty())
		return false;

	// �����۵� ���� �ּҰŸ��� �༮�� �Ǵ��ؼ� targetItem�� �־�����
	float minRange = 10000;
	float tempRange = minRange;
	for (int i = 0; i < _interactItems.size(); i++)
	{
		if (_interactItems[i]->GetScript<Item>()->GetFarmingItem())
		{
			_targetItem = nullptr;
			continue;
		}
		// ��(��)�� ������ ������ �Ÿ��� ���Ѵ�.
		float range = (_transform->GetWorldPosition() - _interactItems[i]->GetTransform()->GetWorldPosition()).Length();
		// ������ minRange�� �־��ش�. 
		tempRange = minRange;
		// ������ �� �ּҰŸ��� �ָ� Ÿ������ ��´�.
		if (range < minRange)
		{
			minRange = range;

			//if (!_interactItems[i]->GetScript<Item>()->GetFarmingItem())
			_targetItem = _interactItems[i];
			/*	else
				{
					minRange = tempRange;
					if (i == 0)
						continue;
					_targetItem = _interactItems[i - 1];

				}*/
		}
	}

	if (_targetItem == nullptr)
		return false;
	return true;
}

bool Player::IsDead()
{

	return false;
}

void Player::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
{
	if (other->GetGameObject()->GetTag() == Tag::Item)
	{
		_interactItems.push_back(other->GetGameObject());
	}
}

void Player::OnTriggerStay(std::shared_ptr<ColliderBase> other) {}

void Player::OnTriggerExit(std::shared_ptr<ColliderBase> other)
{
	if (other->GetGameObject()->GetTag() == Tag::Item)
	{
		auto findIt = find(_interactItems.begin(), _interactItems.end(), other->GetGameObject());

		if (findIt != _interactItems.end())
			_interactItems.erase(findIt);
	}
}
