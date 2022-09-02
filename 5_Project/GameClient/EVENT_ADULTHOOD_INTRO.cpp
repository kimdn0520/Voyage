#include "pch.h"
#include "EVENT_ADULTHOOD_INTRO.h"
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
#include "AdultPlayer.h"
#include "MoveObject.h"
#include "BottlePrefab.h"
#include "CameraScript.h"
#include "BoatPrefab.h"
#include "DataManager.h"
#include "AdultBoyPrefab.h"
#include "AdultGirlPrefab.h"
#include "MoneyPrefab.h"
#include "AdulthoodText.h"
#include "GameManager.h"

#include <random>
#include "DodoYouthPrefab.h"

EVENT_ADULTHOOD_INTRO::EVENT_ADULTHOOD_INTRO(EventMachine* ref)
	: ref(ref)
{

}

EVENT_ADULTHOOD_INTRO::~EVENT_ADULTHOOD_INTRO()
{
	
}

// 하트 이벤트와 동일
void EVENT_ADULTHOOD_INTRO::Start()
{
#pragma region Create
	// 배를 생성한다.
	shared_ptr<BoatPrefab> boatPrefab = make_shared<BoatPrefab>(Vector3(0.f, 3.f, -25.f));

	// 도도를 생성한다. 
	shared_ptr<DodoYouthPrefab> dodoYouthPrefab = make_shared<DodoYouthPrefab>(Vector3(-0.7f, 0.f, 1.5f));
	dodoYouthPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	SceneManager::GetInstance()->SetInstantiateGameObject(dodoYouthPrefab->gameObject);

	// 배를 설정해준다.
	boatPrefab->gameObject->AddComponent<AdultPlayer>();
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetMainCamera(ref->_camera);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetDodo(dodoYouthPrefab->gameObject);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetFishingGaugeBack(boatPrefab->fishingGaugeBack);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetFishingGauge(boatPrefab->fishingGauge);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetFishingRange(boatPrefab->fishingRange);
	ref->_camera->GetScript<CameraScript>()->_targetTransfom = boatPrefab->gameObject->GetComponent<Transform>();
	SceneManager::GetInstance()->SetInstantiateGameObject(boatPrefab->gameObject);

	// 도도의 부모로 배 설정 해준다. 
	dodoYouthPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
	//boatPrefab->gameObject->SetChild(dodoYouthPrefab->gameObject);
	ref->_wave->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());

	// 이벤트머신에 보트 넣어주고~
	ref->_boat = boatPrefab->gameObject;

	// 이벤트머신에 도도도 넣어준다.
	ref->_dodo = dodoYouthPrefab->gameObject;

	ref->_camera->GetTransform()->LookAt(Vector3(0, 0, 0));
	ref->isMove = true;

#pragma endregion Adulthood

	// 어두운 채로 시작!
	ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(1.0f);
	_alpha = 1.0f;
	_isFade = true;
	_talkTime = 0.f;
	_nowText = 0;
	ref->_fadeInOutPanel->GetComponent<Panel>()->SetIsAlpha(_isFade);
	ref->SetEventAdultNextText(0);
	ref->SetNextAdultText(_nowText);
}

int EVENT_ADULTHOOD_INTRO::Update()
{

	if (ref->_boat != nullptr)
		ref->_wave->activeSelf = ref->_boat->GetScript<AdultPlayer>()->MoveCheck();

	// FadeIn ( 점점 밝아 져욧 )
	if (_isFade)
	{
		_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		// FadeIn이 끝났다면..
		if (_alpha <= 0.01f)
		{
			// fade 끝
			_isFade = false;

			// 이벤트 시작
			_isEventCheck = true;
			ref->isFarming = true;

			ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(0.f);
			//ref->_fadeInOutPanel->GetComponent<Panel>()->SetIsAlpha(_isFade);

		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}

	if (_isEventCheck)
	{
		// 파밍이벤트 시작
		Farming();
		ScriptCheck();
		Talk();
	}
	if (ref->isAdultSeaGull)
		return EventMachine::ADULTHOOD_SEAGULL;
	return EventMachine::ADULTHOOD_INTRO;
}

void EVENT_ADULTHOOD_INTRO::End()
{
	ref->isFarming = false;
	_nowText = 0;
	ref->isAdultSeaGull = false;
	ref->SetEventAdultNextText(1);
}

void EVENT_ADULTHOOD_INTRO::Farming()
{
	_curSpawnTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	_curTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	if (_curSpawnTime > 2.f && _curMoneyCnt < 30)
	{
		// TODO : 랜덤하게 머니아이템 생성 서서히 내려오게함
		std::random_device rd;
		std::mt19937 gen(rd());
		uniform_int_distribution<int> dis(-90, 90);

		shared_ptr<MoneyPrefab> heartPrefab = make_shared<MoneyPrefab>(Vector3(dis(gen), 0.f, 100.f));

		SceneManager::GetInstance()->SetInstantiateGameObject(heartPrefab->gameObject);

		_curMoneyCnt++;
		_curSpawnTime = 0.f;
	}

	// 기획에 맞게 고치기
	if (_curTime >= 30.f)
	{	
		ref->isAdultSeaGull = true;
	}
}

void EVENT_ADULTHOOD_INTRO::Talk()
{
	_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;
	if (_talkTime > 2.0f) 
	{
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
		ref->_mainTalkPanel->SetActive(false);
		_talkTime = 0.f;
	}

	if (GameManager::GetInstance()->items[ItemType::Money] == 3 && !_isItem1)
	{
		//_nowText++;
		//ref->SetNextAdultText(_nowText);
		_talkTime = 0.f;
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
		ref->_mainTalkPanel->SetActive(true);
		_isItem1 = true;
	}
	else if (GameManager::GetInstance()->items[ItemType::Money] == 4 && !_isItem2)
	{
		_nowText++;
		_talkTime = 0.f;
		ref->SetNextAdultText(_nowText);
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
		ref->_mainTalkPanel->SetActive(true);
		_isItem2 = true;
	}
	else if (GameManager::GetInstance()->items[ItemType::Money] == 5&& !_isItem3)
	{
		_nowText++;
		_talkTime = 0.f;
		ref->SetNextAdultText(_nowText);
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
		ref->_mainTalkPanel->SetActive(true);
		_isItem3 = true;
	}
}

void EVENT_ADULTHOOD_INTRO::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker1(_nowText);
	ref->SetPanelImage(talker);
}

