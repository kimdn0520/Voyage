#include "pch.h"
#include "AdultPlayer.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Animator.h"
#include "ColliderBase.h"
#include "NoobRay.h"
#include "Camera.h"
#include "SceneManager.h"
#include "Inventory.h"
#include "Item.h"
#include "Panel.h"
#include "Audio.h"
#include "GameProcess.h"
#include "EventMachine.h"
#include "GameManager.h"
#include "Ground.h"

AdultPlayer::AdultPlayer(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject),
	_transform(gameObject->GetTransform()), _timer(0.0f), _fishingtimer(3.0f),
	rotation(Vector3(0.f, 1.f, 0.f)), _speed(20.f), currentState(AdultPlayerState::Idle), _audio(gameObject->GetComponent<Audio>())
{
}

AdultPlayer::~AdultPlayer()
{
}

void AdultPlayer::Awake()
{
	_gameObject = GetGameObject();

	_dodo_Idle = hasher("Nod");
	_dodo_Row = hasher("Rowing");

	_adultBoy_Idle = hasher("Nod");
	_adultBoy_Row = hasher("Rowing");

	_adultGirl_Idle = hasher("Nod");
	_adultGirl_Row = hasher("Rowing.001");

	_grandMa_Idle = hasher("idle");
	_grandMa_Row = hasher("Rowing");
	_grandMa_Die = hasher("Collapse");

	targetPoint = _transform->GetWorldPosition();
	_startRoataion = _transform->GetWorldRotation();

	if(_adultBoy != nullptr)
		_adultBoy->PlayAnim(_adultBoy, _adultBoy_Idle, true);
	
	if(_adultGirl != nullptr)
		_adultGirl->PlayAnim(_adultGirl, _adultGirl_Idle, true);

	if (_grandMa != nullptr)
		_grandMa->PlayAnim(_grandMa, _grandMa_Row, true);
}

void AdultPlayer::FixedUpdate()
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
		case AdultPlayerState::Idle:
		{
			if (_isIdleAnimStart)
			{
				_dodo->PlayAnim(_dodo, _dodo_Idle, true);
				_isIdleAnimStart = false;
			}
			_moveCheck = false;

			Idle();
		}
		break;
		case AdultPlayerState::Move:
		{
			if (_isMoveAnimStart)
			{
				_dodo->PlayAnim(_dodo, _dodo_Row, true);
				_isMoveAnimStart = false;
			}
			_moveCheck = true;

			Move();
		}
		break;
		case AdultPlayerState::Fishing:
		{
			// TODO : 낚시함수 작성 (몇초후에 타겟아이템 사라지게하기)
			GameManager::GetInstance()->isPlayerStop = true;
			Fishing();
			_moveCheck = false;

		}
		break;
		case AdultPlayerState::Talk:
		{
			Talk();
		}
		break;
		}
		_isIdleAnimCheck = true;
	}
	else
	{
		currentState = AdultPlayerState::Idle;
		_isIdleAnimStart = true;

		if (_isIdleAnimStart && _isIdleAnimCheck)
		{
			if (_adultBoy != nullptr)
				_adultBoy->PlayAnim(_adultBoy, _adultBoy_Idle, true);
			
			if(_adultGirl != nullptr)
				_adultGirl->PlayAnim(_adultGirl, _adultGirl_Idle, true);

			/*if(_grandMa != nullptr)
				_grandMa->PlayAnim(_grandMa, _grandMa_Idle, true);*/

			if(_dodo != nullptr)
				_dodo->PlayAnim(_dodo, _dodo_Idle, true);

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
			if (_targetItem != nullptr && !_targetItem->GetScript<Item>()->GetFarmingItem())
				Destroy(_targetItem);
		}
		Idle();
	}
}

void AdultPlayer::Update()
{

}

void AdultPlayer::Idle()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::SPACE))
	{
		if (TargetItem())
		{
			// state를 fishing으로 바꿔준다.
			currentState = AdultPlayerState::Fishing;

			// fishingGauge UI를켜준다.
			_fishingGauge->activeSelf = true;
			_fishingGaugeBack->activeSelf = true;

			// 낚시중인 아이템을 멈춘다.
			_targetItem->GetScript<Item>()->SetIsMove(false);
		}
	}

	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		NoobRay ray = _mainCamera->GetCamera()->ScreenPointToRay(InputManager::GetInstance()->GetMousePos());

		if (ray.Intersect())
		{
			currentPoint = ray.point;
			currentPoint.y = _transform->GetWorldPosition().y;
		}
	}
	else if (InputManager::GetInstance()->GetButton(KEY_TYPE::LBUTTON))
	{
		NoobRay ray = _mainCamera->GetCamera()->ScreenPointToRay(InputManager::GetInstance()->GetMousePos());

		if (ray.Intersect())
		{
			targetPoint = ray.point;
			targetPoint.y = _transform->GetWorldPosition().y;
		}
	}
	else if (InputManager::GetInstance()->GetButtonUp(KEY_TYPE::LBUTTON))
	{
		_dir = targetPoint - currentPoint;
		_dist = (targetPoint - currentPoint).Length();

		currentState = AdultPlayerState::Move;
		_isMoveAnimStart = true;

		GameManager::GetInstance()->isRow = true;
	}

	//ShakeObject(); // 앞뒤로 배가 움직이는것처럼 움직임
}

void AdultPlayer::Move()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::SPACE))
	{
		/*GetGameObject()->GetComponent<Audio>()->PlayEvent("event:/Weapons/Explosion");*/

		if (TargetItem())
		{
			currentState = AdultPlayerState::Fishing;

			// fishingGauge UI를켜준다.
			_fishingGauge->activeSelf = true;
			_fishingGaugeBack->activeSelf = true;

			_dodo->PlayAnim(_dodo, _dodo_Idle, true);
			/*_adultBoy->PlayAnim(_adultBoy, _adultBoy_Idle);
			_adultGirl->PlayAnim(_adultGirl, _adultGirl_Idle);*/

			// 낚시중인 아이템을 멈춘다.
			_targetItem->GetScript<Item>()->SetIsMove(false);
		}
	}

	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		NoobRay ray = _mainCamera->GetCamera()->ScreenPointToRay(InputManager::GetInstance()->GetMousePos());

		if (ray.Intersect())
		{
			currentPoint = ray.point;
			currentPoint.y = _transform->GetWorldPosition().y;
		}
	}
	else if (InputManager::GetInstance()->GetButton(KEY_TYPE::LBUTTON))
	{
		NoobRay ray = _mainCamera->GetCamera()->ScreenPointToRay(InputManager::GetInstance()->GetMousePos());

		if (ray.Intersect())
		{
			targetPoint = ray.point;
			targetPoint.y = _transform->GetWorldPosition().y;
		}
	}
	else if (InputManager::GetInstance()->GetButtonUp(KEY_TYPE::LBUTTON))
	{
		_dir = targetPoint - currentPoint;
		_dist = (targetPoint - currentPoint).Length();

		 currentState = AdultPlayerState::Move;
		_distToPoint = 0.f;

		GameManager::GetInstance()->isRow = true;
	}

	if (_distToPoint < _dist)
	{
		GameManager::GetInstance()->isPlayerStop = false;

		// 정규화
		_dir.Normalize();

		// 찍은 지점으로 이동해야할 속도? 를 게임매니저에 건네준다.
		GameManager::GetInstance()->SetObjectMove(_dir * _speed * TimeManager::GetInstance()->GetDeltaTime());

		// 움직인 거리를 지속적으로 저장
		_distToPoint += (_dir * _speed * TimeManager::GetInstance()->GetDeltaTime()).Length();

		if(currentPoint != targetPoint)
			_transform->LookAt(_transform->GetWorldPosition() + _dir);
	}
	else
	{
		currentState = AdultPlayerState::Idle;
		_isIdleAnimStart = true;

		_distToPoint = 0.f;
		GameManager::GetInstance()->isPlayerStop = true;
	}
}

void AdultPlayer::ShakeObject()
{
	_frequency += TimeManager::GetInstance()->GetDeltaTime();

	float sinFrequnecy = sinf(_frequency);

	_transform->MoveLook(1.0f * sinFrequnecy * TimeManager::GetInstance()->GetDeltaTime());
}

void AdultPlayer::Talk()
{
}

int AdultPlayer::ShakingBoat(bool check, bool right)
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

void AdultPlayer::Fishing()
{
	if (_isIdleAnimStart)
	{
		/*_adultBoy->PlayAnim(_adultBoy, _adultBoy_Idle);
		_adultGirl->PlayAnim(_adultGirl, _adultGirl_Idle);*/
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

		if(_targetItem->GetScript<Item>()->GetItemType() == ItemType::BigStar)
			EventMachine::GetInstance()->isYouthIndependent = true;

		// 파괴한다.
		Destroy(_targetItem);

		// state Idle로 바꿔준다.
		currentState = AdultPlayerState::Idle;

		_isIdleAnimStart = true;

		_timer = 0;
		
		_audio->PlayEvent("event:/SFX/SFX_Item2", false);
	}
}

bool AdultPlayer::ShakingEvent(bool check, bool right)
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
				if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::Z))
				{
					_transform->Rotate(Vector3(0.f, 0.f, 10.f));
					_introTotalTime = 0.f;
					return true;
				}
			}
			// 왼쪽으로 기울이고 있으면 오른쪽 키입력을 받아야한다. 
			else
			{
				if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::C))
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

			GameManager::GetInstance()->items[ItemType::Heart] -= 5;

			if (GameManager::GetInstance()->items[ItemType::Heart] < 0)
			{
				GameManager::GetInstance()->items[ItemType::Heart] = 0;
			}

			_introTotalTime = 0.f;
			return false;
		}
	}
}

bool AdultPlayer::TargetItem()
{
	if (_interactItems.empty())
		return false;

	// 아이템들 중의 최소거리인 녀석을 판단해서 targetItem에 넣어주자
	float minRange = 10000;

	for (int i = 0; i < _interactItems.size(); i++)
	{

		if (_interactItems[i]->GetScript<Item>()->GetFarmingItem())
		{
			_targetItem = nullptr;
			continue;
		}
		// 나(배)와 아이템 사이의 거리를 구한다.
		float range = (_transform->GetWorldPosition() - _interactItems[i]->GetTransform()->GetWorldPosition()).Length();

		// 아이템 중 최소거리인 애를 타겟으로 삼는다.
		if (range < minRange)
		{
			minRange = range;

			_targetItem = _interactItems[i];
		}
	}

	if (_targetItem == nullptr)
		return false;
	return true;
}

void AdultPlayer::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
{
	if (other->GetGameObject()->GetTag() == Tag::Item)
	{
		_interactItems.push_back(other->GetGameObject());
	}
	else if (other->GetGameObject()->GetTag() == Tag::Ground)
	{
		// 갈매기 이벤트 시작
		EventMachine::GetInstance()->isAdultSeaGull = true;
		
		// 플레이어 움직임 봉쇄
		EventMachine::GetInstance()->isMove = false;

		// 갈매기랑 섬 움직임 봉쇄
		other->GetGameObject()->GetScript<Ground>()->SetIsMove(false);
		
		// 도도 idle 상태로
		_dodo->PlayAnim(_dodo, _dodo_Idle, true);
	}
	else if (other->GetGameObject()->GetTag() == Tag::Ground2)
	{
		// 거북이 이벤트 시작
		EventMachine::GetInstance()->isSellDreamTalk = true;

		// 플레이어 움직임 봉쇄
		EventMachine::GetInstance()->isMove = false;

		// 거북이랑 섬 움직임 봉쇄
		other->GetGameObject()->GetScript<Ground>()->SetIsMove(false);

		// 도도 idle 상태로
		_dodo->PlayAnim(_dodo, _dodo_Idle, true);
	}
	else if (other->GetGameObject()->GetTag() == Tag::Disaster)
	{
		disaster = other->GetGameObject();

		GameManager::GetInstance()->items[ItemType::Heart] -= 3;

		if (GameManager::GetInstance()->items[ItemType::Heart] <= 0)
		{
			GameManager::GetInstance()->items[ItemType::Heart] = 0;
		}
	}
	else if (other->GetGameObject()->GetTag() == Tag::BadEnding)
	{
		// 플레이어 움직임 봉쇄
		EventMachine::GetInstance()->isMove = false;

		// 섬 움직임 봉쇄
		other->GetGameObject()->GetScript<Ground>()->SetIsMove(false);

		EventMachine::GetInstance()->isBadEnding = true;
	}
}

void AdultPlayer::OnTriggerStay(std::shared_ptr<ColliderBase> other) 
{
	_beAttackedTime += TimeManager::GetInstance()->GetDeltaTime();

	if (other->GetGameObject()->GetTag() == Tag::Enemy)
	{
		if (_beAttackedTime >= 1.f)
		{
			GameManager::GetInstance()->items[ItemType::Heart] -= 3;

			if (GameManager::GetInstance()->items[ItemType::Heart] <= 0)
			{
				GameManager::GetInstance()->items[ItemType::Heart] = 0;
			}

			_beAttackedTime = 0.f;
		}
	}
	else if (other->GetGameObject()->GetTag() == Tag::Disaster)
	{
		if (_beAttackedTime >= 1.f)
		{
			GameManager::GetInstance()->items[ItemType::Heart] -= 3;

			if (GameManager::GetInstance()->items[ItemType::Heart] <= 0)
			{
				GameManager::GetInstance()->items[ItemType::Heart] = 0;
			}

			_beAttackedTime = 0.f;
		}
	}
}

void AdultPlayer::OnTriggerExit(std::shared_ptr<ColliderBase> other)
{
	if(other->GetGameObject() == nullptr)
		return;

	if (other->GetGameObject()->GetTag() == Tag::Item)
	{
		auto findIt = find(_interactItems.begin(), _interactItems.end(), other->GetGameObject());

		if (findIt != _interactItems.end())
			_interactItems.erase(findIt);
	}
	else if (other->GetGameObject()->GetTag() == Tag::Enemy)
	{
		GameManager::GetInstance()->items[ItemType::Heart] -= 3;

		if (GameManager::GetInstance()->items[ItemType::Heart] <= 0)
		{
			GameManager::GetInstance()->items[ItemType::Heart] = 0;
		}
	}
}
