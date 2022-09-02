#include "pch.h"
#include "EVENT_YOUTH_FARMING.h"
#include "EventMachine.h"
#include "IEventState.h"
#include "GameProcess.h"
#include "Transform.h"
#include "GameObject.h"
#include "Panel.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "player.h"
#include "MoveObject.h"
#include "BottlePrefab.h"
#include "CameraScript.h"
#include "BoatPrefab.h"
#include "GameManager.h"
#include "YouthText.h"
#include <random>
#include "StarPrefab.h"
#include "BigStarPrefab.h"
#include "AdultPlayer.h"
#include "Item.h"
#include "Transform.h"
#include "GameObject.h"

EVENT_YOUTH_FARMING::EVENT_YOUTH_FARMING(EventMachine* ref)
	: ref(ref)
{

}

EVENT_YOUTH_FARMING::~EVENT_YOUTH_FARMING()
{}

void EVENT_YOUTH_FARMING::Start()
{
	ref->SetNextYouthtText(0);
	ref->SetEventYouthNextText(2);
	ScriptCheck();

	ref->_mainTalkPanel->GetScript<YouthText>()->_isPanelCheck = true;
	ref->_mainTalkPanel->SetActive(true);

	_talkTime = 0.f;
	_isTalk = true;

	// 대사가 끝날때까지 움직임을 멈춘다.
	ref->isMove = false;
	GameManager::GetInstance()->isPlayerStop = true;
	_isStar1 = false;
	_isStar2 = false;
	_isStar3 = false;
	_isStar4 = false;
	_isStar5 = false;
}

int EVENT_YOUTH_FARMING::Update()
{
	if (SkipNextScene())
		return EventMachine::YOUTH_SOLO;


	if (ref->_boat != nullptr)
		ref->_wave->activeSelf = ref->_boat->GetScript<AdultPlayer>()->MoveCheck();

	FarmingItem();
	// AdultPlayer 스크립트에서 큰별을 먹었다면 이걸 true로 바꿔줌
	if (ref->isYouthIndependent)
	{
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha >= 1.0f)
		{
			ref->_fadeInOutPanel->GetComponent<Panel>()->SetIsAlpha(1.0f);

			return EventMachine::YOUTH_SOLO;
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}
	else
	{
		IntroScript();

		_curSpawnTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		if (_curSpawnTime > 2.f && _curStarCnt < 30)
		{
			// TODO : 랜덤하게 하트아이템 생성 서서히 내려오게함
			std::random_device rd;
			std::mt19937 gen(rd());
			uniform_int_distribution<int> dis(-90, 90);

			shared_ptr<StarPrefab> starPrefab = make_shared<StarPrefab>(Vector3(dis(gen), 0.5f, 100.f));

			SceneManager::GetInstance()->SetInstantiateGameObject(starPrefab->gameObject);

			_curStarCnt++;
			_curSpawnTime = 0.f;

			if (_curStarCnt == 30)
			{
				shared_ptr<BigStarPrefab> bigStarPrefab = make_shared<BigStarPrefab>(Vector3(0.f, 0.5f, 100.f));

				SceneManager::GetInstance()->SetInstantiateGameObject(bigStarPrefab->gameObject);
			}
		}

		// 별 스폰을 다했다면 큰별을 15초마다 스폰한다. 
		if (_curStarCnt == 30 && _curSpawnTime > 15.f)
		{
			shared_ptr<BigStarPrefab> bigStarPrefab = make_shared<BigStarPrefab>(Vector3(0.f, 0.5f, 100.f));

			SceneManager::GetInstance()->SetInstantiateGameObject(bigStarPrefab->gameObject);

			_curSpawnTime = 0;
		}
	}

	return EventMachine::YOUTH_FARMING;
}

void EVENT_YOUTH_FARMING::End()
{
	ref->isFarming = false;

	ref->isYouthIndependent = false;

	_isStar1 = false;
	_isStar2 = false;
	_isStar3 = false;
	_isStar4 = false;
	_isStar5 = false;

	for (auto star : _starPrefab)
		SceneManager::GetInstance()->SetRemoveGameObject(star->gameObject);
}

void EVENT_YOUTH_FARMING::IntroScript()
{
	if (_isTalk)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		// 3초마다 말하는중
		if (_talkTime > 3.0f)
		{
			ScriptCheck();

			ref->SetNextYouthtText(_nowText);
			_talkTime = 0.f;
			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<YouthText>()->Text3Size() == _nowText)
		{
			_isTalk = false;

			ref->_mainTalkPanel->GetScript<YouthText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

			// 대사가 끝났다면 다시 움직여줌
			ref->isMove = true;
		}
	}
}

void EVENT_YOUTH_FARMING::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<YouthText>()->ReturnTalker3(_nowText);
	ref->SetPanelImage(talker);
}

void EVENT_YOUTH_FARMING::FarmingItem()
{
	if (GameManager::GetInstance()->items[ItemType::Star] == 3 && !_isStar1)
	{
		_starPrefab.push_back(make_shared<StarPrefab>(Vector3(0.f, 2.f, 0.f)));
		_starPrefab[_starIndex]->gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
		_starPrefab[_starIndex]->gameObject->GetScript<Item>()->SetIsFarmingItem(true);
		SceneManager::GetInstance()->SetInstantiateGameObject(_starPrefab[_starIndex]->gameObject);
		_starPrefab[_starIndex]->gameObject->GetComponent<Transform>()->SetParent(ref->_boat->GetTransform());
		_isStar1 = true;
		_starIndex++;
	}
	else if (GameManager::GetInstance()->items[ItemType::Star] == 6 && !_isStar2)
	{
		_starPrefab.push_back(make_shared<StarPrefab>(Vector3(0.5f, 2.f, 0.f)));
		_starPrefab[_starIndex]->gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
		_starPrefab[_starIndex]->gameObject->GetScript<Item>()->SetIsFarmingItem(true);
		SceneManager::GetInstance()->SetInstantiateGameObject(_starPrefab[_starIndex]->gameObject);
		_starPrefab[_starIndex]->gameObject->GetComponent<Transform>()->SetParent(ref->_boat->GetTransform());
		_isStar2 = true;
		_starIndex++;
	}
	else if (GameManager::GetInstance()->items[ItemType::Star] == 9 && !_isStar3)
	{
		_starPrefab.push_back(make_shared<StarPrefab>(Vector3(-0.5f, 2.f, 0.f)));
		_starPrefab[_starIndex]->gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
		_starPrefab[_starIndex]->gameObject->GetScript<Item>()->SetIsFarmingItem(true);
		SceneManager::GetInstance()->SetInstantiateGameObject(_starPrefab[_starIndex]->gameObject);
		_starPrefab[_starIndex]->gameObject->GetComponent<Transform>()->SetParent(ref->_boat->GetTransform());
		_isStar3= true;
		_starIndex++;
	}
	else if (GameManager::GetInstance()->items[ItemType::Star] == 12 && !_isStar4)
	{
		_starPrefab.push_back(make_shared<StarPrefab>(Vector3(-0.5f, 2.f, -1.f)));
		_starPrefab[_starIndex]->gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
		_starPrefab[_starIndex]->gameObject->GetScript<Item>()->SetIsFarmingItem(true);
		SceneManager::GetInstance()->SetInstantiateGameObject(_starPrefab[_starIndex]->gameObject);
		_starPrefab[_starIndex]->gameObject->GetComponent<Transform>()->SetParent(ref->_boat->GetTransform());
		_isStar4 = true;
		_starIndex++;
	}
	else if (GameManager::GetInstance()->items[ItemType::Star] == 15 && !_isStar5)
	{
		_starPrefab.push_back(make_shared<StarPrefab>(Vector3(0.5f, 2.f, -1.f)));
		_starPrefab[_starIndex]->gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
		_starPrefab[_starIndex]->gameObject->GetScript<Item>()->SetIsFarmingItem(true);
		SceneManager::GetInstance()->SetInstantiateGameObject(_starPrefab[_starIndex]->gameObject);
		_starPrefab[_starIndex]->gameObject->GetComponent<Transform>()->SetParent(ref->_boat->GetTransform());
		_isStar5 = true;
		_starIndex++;
	}

}

bool EVENT_YOUTH_FARMING::SkipNextScene()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F3))
	{
		return true;
	}
	return false;

}
