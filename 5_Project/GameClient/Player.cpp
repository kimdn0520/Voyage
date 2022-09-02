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
			// TODO : 낚시함수 작성 (몇초후에 타겟아이템 사라지게하기)
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
			// fishingGauge UI를 꺼준다.
			_fishingGauge->activeSelf = false;
			_fishingGaugeBack->activeSelf = false;

			// 파괴한다.
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
				// state를 fishing으로 바꿔준다.
				currentState = PlayerState::Fishing;

				// fishingGauge UI를켜준다.
				_fishingGauge->activeSelf = true;
				_fishingGaugeBack->activeSelf = true;
				// 낚시중인 아이템을 멈춘다.
				_targetItem->GetScript<Item>()->SetIsMove(false);
			}
		}
	}

	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		//Screen에서 받은 mousePos를 ViewSpace로 바꾸어서 [rayOrigin : 광선 쏘는 시작지점, direction : 광선의 방향] 을 ray에 return 함
		NoobRay ray = _mainCamera->GetCamera()->ScreenPointToRay(InputManager::GetInstance()->GetMousePos());

		// ray랑 부딪힌게 있는지 판단해주는 함수
		// 있다면 point에 그 지점 World 형태로 저장된다.
		if (ray.Intersect())
		{
			targetPoint = ray.point;
			targetPoint.y = _transform->GetWorldPosition().y;		// y는 그대로해서 움직여야징~희

			currentState = PlayerState::Move;
			_isMoveAnimStart = true;
		}
	}


	//	ShakeObject(); // 앞뒤로 배가 움직이는것처럼 움직임
}

void Player::Move()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::SPACE))
	{
		/*GetGameObject()->GetComponent<Audio>()->PlayEvent("event:/Weapons/Explosion");*/

		if (TargetItem())
		{
			currentState = PlayerState::Fishing;

			// fishingGauge UI를켜준다.
			if (!_targetItem->GetScript<Item>()->GetFarmingItem())
			{
				_fishingGauge->activeSelf = true;
				_fishingGaugeBack->activeSelf = true;
			}
			_adultBoy->PlayAnim(_adultBoy, _adultBoy_Idle, true);
			_adultGirl->PlayAnim(_adultGirl, _adultGirl_Idle, true);

			// 낚시중인 아이템을 멈춘다.
			_targetItem->GetScript<Item>()->SetIsMove(false);
		}
	}

	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		//Screen에서 받은 mousePos를 ViewSpace로 바꾸어서 [rayOrigin : 광선 쏘는 시작지점, direction : 광선의 방향] 을 ray에 return 함
		NoobRay ray = _mainCamera->GetCamera()->ScreenPointToRay(InputManager::GetInstance()->GetMousePos());

		// ray랑 부딪힌게 있는지 판단해주는 함수
		// 있다면 point에 그 지점 World 형태로 저장된다.
		if (ray.Intersect())
		{
			targetPoint = ray.point;
			targetPoint.y = _transform->GetWorldPosition().y;		// y는 그대로해서 움직여야징~희

			currentState = PlayerState::Move;

			// 움직이는 도중에 움직여서 targetPoint가 바뀌어서 dist가 바뀌었다 그래서
			// distToPoint를 초기화 해준다.
			_distToPoint = 0.f;
		}
	}

	// 일단 이동해야할 거리를 저장해둔다.
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

		// 해당 벡터의 각도를 atn2로 구하고 rad이니 degree로 바꾸고 나 자신이 돌아야하니까 90에서 빼준다.
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

		// 찍은 지점으로 이동해야할 속도 를 건네준다.
		GameManager::GetInstance()->SetObjectMove(dir * _speed * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed);

		// 움직인 거리를 지속적으로 저장
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
		// UI게이지는 다시 0으로
		_fishingGauge->GetComponent<Panel>()->SetOffset(
			GameProcess::_windowInfo.width / 2 - 50.f,
			GameProcess::_windowInfo.width / 2 - 50.f,
			GameProcess::_windowInfo.height / 2 + 50.f,
			GameProcess::_windowInfo.height / 2 + 70.f);
		_fishingGauge->GetComponent<Panel>()->screenResize = Vector2(1920.f, 1080.f);
		_fishingGauge->GetComponent<Panel>()->Resize(GameProcess::resizeInfo.width, GameProcess::resizeInfo.height);

		// fishingGauge UI를 꺼준다.
		_fishingGauge->activeSelf = false;
		_fishingGaugeBack->activeSelf = false;

		// 게임매니저에 낚시한 아이템 갯수를 증가시켜준다. 
		GameManager::GetInstance()->items[_targetItem->GetScript<Item>()->GetItemType()]++;

		// 파괴한다.
		if (!_targetItem->GetScript<Item>()->GetFarmingItem())
			Destroy(_targetItem);

		// state Idle로 바꿔준다.
		currentState = PlayerState::Idle;

		_isIdleAnimStart = true;

		_timer = 0;

		_audio->PlayEvent("event:/SFX/SFX_Item2", false);
	}
}

bool Player::ShakingEvent(bool check, bool right)
{
	// 이번트가 실행된다고 들어오면 흔들린다. 
	if (check)
	{
		_introTotalTime += TimeManager::GetInstance()->GetDeltaTime();

		// 2초보다 적다면 키 입력을 받게한다. 
		if (5.0f > _introTotalTime)
		{
			// 오른쪽으로 기울이고 있으면 왼쪽 키입력을 받아야한다. 
			if (right)
			{
				if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::Q))
				{
					_transform->Rotate(Vector3(0.f, 0.f, 10.f));
					_introTotalTime = 0.f;
					return true;
				}
			}
			// 왼쪽으로 기울이고 있으면 오른쪽 키입력을 받아야한다. 
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
			// 원래대로 돌리고 체력을 -5 해준다.
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

	// 아이템들 중의 최소거리인 녀석을 판단해서 targetItem에 넣어주자
	float minRange = 10000;
	float tempRange = minRange;
	for (int i = 0; i < _interactItems.size(); i++)
	{
		if (_interactItems[i]->GetScript<Item>()->GetFarmingItem())
		{
			_targetItem = nullptr;
			continue;
		}
		// 나(배)와 아이템 사이의 거리를 구한다.
		float range = (_transform->GetWorldPosition() - _interactItems[i]->GetTransform()->GetWorldPosition()).Length();
		// 그전에 minRange를 넣어준다. 
		tempRange = minRange;
		// 아이템 중 최소거리인 애를 타겟으로 삼는다.
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
