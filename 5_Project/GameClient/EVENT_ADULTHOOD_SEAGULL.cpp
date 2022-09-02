#include "pch.h"
#include "EVENT_ADULTHOOD_SEAGULL.h"
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
#include "IslandPrefab.h"
#include "CameraScript.h"
#include "BoatPrefab.h"
#include "DataManager.h"
#include "AdultBoyPrefab.h"
#include "AdultGirlPrefab.h"
#include "BirdPrefab.h"
#include "AdulthoodText.h"
#include "GameManager.h"
#include "DodoYouthPrefab.h"
#include "BigBoatPrefab.h"

EVENT_ADULTHOOD_SEAGULL::EVENT_ADULTHOOD_SEAGULL(EventMachine* ref)
	: ref(ref)
{

}

EVENT_ADULTHOOD_SEAGULL::~EVENT_ADULTHOOD_SEAGULL()
{}

void EVENT_ADULTHOOD_SEAGULL::Start()
{
	// 섬을 생성한다. 
	_islandPrefab = make_shared<IslandPrefab>(Vector3(-50.f, 0.f, 150.f));
	//islandPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(4.f, 4.f, 4.f));
	SceneManager::GetInstance()->SetInstantiateGameObject(_islandPrefab->gameObject);

	// 갈매기를 생성한다. 
	_birdPrefab = make_shared<BirdPrefab>(Vector3(1.f, -2.f, 2.f));
	_birdPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(2.f, 2.f, 2.f));
	_birdPrefab->gameObject->GetTransform()->SetLocalRotation(Vector3(30.f, 165.f, 0.f));
	SceneManager::GetInstance()->SetInstantiateGameObject(_birdPrefab->gameObject);

	// 섬에 자식으로 갈매기를 넣는다. 
	_birdPrefab->gameObject->GetComponent<Transform>()->SetParent(_islandPrefab->gameObject->GetTransform());

	// 이번에 나올 텍스트
	_nowText = 0;
	ref->SetEventAdultNextText(1);
	ref->SetNextAdultText(_nowText);

	_blinkTime = 5.f;
	_isTalk = true;
}

int EVENT_ADULTHOOD_SEAGULL::Update()
{
	if (ref->_boat != nullptr)
		ref->_wave->activeSelf = ref->_boat->GetScript<AdultPlayer>()->MoveCheck();

	// 갈매기와 충돌시 이벤트 시작
	if (ref->isAdultSeaGull)
	{
		if (_isSeagullTalk)
		{	
			CameraMove();
		}
		else
		{
			IntroScript();
		}
	}

	// 갈매기와 이야기가 끝난후 -> 큰 배를 탄다 -> 페이드 아웃 -> 다음 이벤트 온
	if (_isFade)
	{
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha >= 1.0f)
		{
			_isFade = false;

			// 다음 경쟁자 이벤트 true
			ref->isAdultCompetitor = true;

			_alpha = 1.0f;

			// 갈매기와 섬을 파괴한다.
			SceneManager::GetInstance()->SetRemoveGameObject(_islandPrefab->gameObject);
			_islandPrefab.reset();
			SceneManager::GetInstance()->SetRemoveGameObject(_birdPrefab->gameObject);
			_birdPrefab.reset();
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}

	// 다음 이벤트인 경쟁자 이벤트로..
	if(ref->isAdultCompetitor)
		return EventMachine::ADULTHOOD_COMPETITOR;

	return EventMachine::ADULTHOOD_SEAGULL;
}

void EVENT_ADULTHOOD_SEAGULL::End()
{
	// 다시 움직일 수 있게하고
	ref->isMove = true;

	// 조건들 다시 false
	ref->isAdultCompetitor = false;
}

void EVENT_ADULTHOOD_SEAGULL::IntroScript()
{
	if (_isTalk)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		// 2초마다 말하는중
		if (_talkTime > 2.0f)
		{
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
			ref->_mainTalkPanel->SetActive(true);

			ScriptCheck();

			ref->SetNextAdultText(_nowText);
			_talkTime = 0.f;
			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text2Size() == _nowText)
		{
			_isTalk = false;

			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

			// 원래 있던 배랑 도도를 삭제해야 한다.
			SceneManager::GetInstance()->SetRemoveGameObject(ref->_boat);
			SceneManager::GetInstance()->SetRemoveGameObject(ref->_dodo);
			ref->_boat.reset();
			ref->_dodo.reset();

			// 배를 생성한다.
			shared_ptr<BigBoatPrefab> boatPrefab = make_shared<BigBoatPrefab>(Vector3(0.f, 3.f, -25.f));

			// 도도를 생성한다. 
			shared_ptr<DodoYouthPrefab> dodoYouthPrefab = make_shared<DodoYouthPrefab>(Vector3(-0.7f, 5.f, 1.5f));
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

			ref->_wave->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f,0.f,-12.8f));

			// 도도의 부모로 배 설정 해준다. 
			dodoYouthPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
			ref->_wave->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());

			// 이벤트머신에 보트 넣어주고~
			ref->_boat = boatPrefab->gameObject;

			// 카메라 위치 셋팅
			ref->_camera->GetTransform()->SetLocalPosition(Vector3(0.f, 150.f ,0.f));
			ref->_camera->GetTransform()->LookAt(Vector3(0, 0, 0));

			_isFade = true;
		}
	}
}

void EVENT_ADULTHOOD_SEAGULL::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker2(_nowText);
	ref->SetPanelImage(talker);
}

void EVENT_ADULTHOOD_SEAGULL::CameraMove()
{
	if (_seagullSumCameraTrans <= _seagullCameraTrans)
	{
		ref->_camera->GetTransform()->MoveLook(12.f * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed);
		ref->_camera->GetTransform()->MoveUp(-5.f * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed);
		ref->_camera->GetTransform()->MoveRight(-3.f * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed);

		_seagullSumCameraTrans += 10.f * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;
	}
	else
	{
		float rotateVal = 0.5f * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;
		_totalCameraTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		if (_seagullSumCamerRotateX <= _seagullCamerRotateX)
		{
			if (_totalCameraTime > 0.01f)
			{
				// 카메라 올려줌 
				_seagullSumCamerRotateX += rotateVal;
				ref->_camera->GetTransform()->RotateXAxis(-rotateVal);
				_totalCameraTime = 0.f;
			}
		}
		else
		{
			_isSeagullTalk = false;
		}
	}
}