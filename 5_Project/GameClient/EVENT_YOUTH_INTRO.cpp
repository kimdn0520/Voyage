#include "pch.h"
#include "EVENT_YOUTH_INTRO.h"
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
#include "YouthText.h"
#include "NewbieEngineFunc.h"
#include "DodoYouthPrefab.h"
#include "InputManager.h"
#include "GameManager.h"

EVENT_YOUTH_INTRO::EVENT_YOUTH_INTRO(EventMachine* ref)
	: ref(ref)
{

}

EVENT_YOUTH_INTRO::~EVENT_YOUTH_INTRO()
{}

void EVENT_YOUTH_INTRO::Start()
{
	// 아빠를 생성한다. 
	shared_ptr<AdultBoyPrefab> adultBoyPrefab = make_shared<AdultBoyPrefab>(Vector3(0.7f, 0.f, -2.f));
	adultBoyPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	SceneManager::GetInstance()->GetActiveScene()->AddGameObject(adultBoyPrefab->gameObject);

	// 엄마를 생성한다. 
	shared_ptr<AdultGirlPrefab> adultGirlPrefab = make_shared<AdultGirlPrefab>(Vector3(-0.7f, 0.f, -2.f));
	adultGirlPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	SceneManager::GetInstance()->GetActiveScene()->AddGameObject(adultGirlPrefab->gameObject);

	// 도도를 생성한다. 
	shared_ptr<DodoYouthPrefab> dodoYouthPrefab = make_shared<DodoYouthPrefab>(Vector3(-0.7f, 0.f, 2.f));
	dodoYouthPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	SceneManager::GetInstance()->GetActiveScene()->AddGameObject(dodoYouthPrefab->gameObject);

	// 배를 생성한다.
	shared_ptr<BoatPrefab> boatPrefab = make_shared<BoatPrefab>(Vector3(0.f, 3.f, -25.f));
	// 터치 드래그하는 스크립트는 다른걸 쓸까..?
	boatPrefab->gameObject->AddComponent<AdultPlayer>();
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetMainCamera(ref->_camera);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetAdultBoy(adultBoyPrefab->gameObject);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetAdultGirl(adultGirlPrefab->gameObject);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetDodo(dodoYouthPrefab->gameObject);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetFishingGaugeBack(boatPrefab->fishingGaugeBack);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetFishingGauge(boatPrefab->fishingGauge);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetFishingRange(boatPrefab->fishingRange);
	ref->_camera->GetScript<CameraScript>()->_targetTransfom = boatPrefab->gameObject->GetComponent<Transform>();
	SceneManager::GetInstance()->SetInstantiateGameObject(boatPrefab->gameObject);

	// 플레이어의 자식에 배를 넣어준다. 
	adultBoyPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
	adultGirlPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
	dodoYouthPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
	ref->_wave->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());

	ref->_boat = boatPrefab->gameObject;
	ref->_camera->GetTransform()->LookAt(Vector3(0, 0, 0));

	// 어두운 채로 시작!
	ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(1.0f);
	_alpha = 1.0f;
	_isFade = true;
	ref->_fadeInOutPanel->GetComponent<Panel>()->SetIsAlpha(_isFade);

	// 이번에 나올 텍스트
	ref->SetEventYouthNextText(0);
	ref->SetNextYouthtText(_nowText);

	// 플레이어 움직임 막기
	ref->isMove = false;
}

int EVENT_YOUTH_INTRO::Update()
{
	if (SkipNextScene())
	{
		return EventMachine::YOUTH_ROW;
	}

	// FadeIn ( 점점 밝아 져욧 )
	if (_isFade)
	{
		_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		// FadeIn이 끝났다면..
		if (_alpha <= 0.01f)
		{
			// fade 끝
			_isFade = false;

			// 말하기 시작
			_isTalk = true;

			ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(0.f);
			//ref->_fadeInOutPanel->GetComponent<Panel>()->SetIsAlpha(_isFade);
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}
	if (ref->_boat->GetComponent<AdultPlayer>() != nullptr)

	if (ref->_boat->GetComponent<AdultPlayer>() !=nullptr)
		ref->_wave->activeSelf = ref->_boat->GetScript<AdultPlayer>()->MoveCheck();

	ScriptCheck();
	IntroScript();

	// 대화가 끝났다면 true가 되고 노젓기 튜토리얼 이벤트로..
	if (ref->isYouthRow)
	{
		return EventMachine::YOUTH_ROW;
	}

	return EventMachine::YOUTH_INTRO;
}

void EVENT_YOUTH_INTRO::End()
{
	ref->isYouthIntro = false;
	ref->isYouthRow = false;

	// 플레이어 다시 움직이게하기
	ref->isMove = true;
}

void EVENT_YOUTH_INTRO::IntroScript()
{
	if (_isTalk)
	{
		ref->_mainTalkPanel->SetActive(true);
		ref->_mainTalkPanel->GetScript<YouthText>()->_isPanelCheck = true;
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		// 3초마다 말하는중
		if (_talkTime > 3.0f)
		{
			_nowText++;
			ref->SetNextYouthtText(_nowText);
			_talkTime = 0.f;
		}

		// 말하기가 끝났다면
		if (ref->_mainTalkPanel->GetScript<YouthText>()->Text1Size() == _nowText)
		{
			ref->_mainTalkPanel->GetScript<YouthText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

			_isTalk = false;

			ref->isYouthRow = true;			// 노젓기 튜토리얼로 이동
		}
	}
}

void EVENT_YOUTH_INTRO::ScriptCheck()
{
	if (_isTalk)
	{
		int talker = ref->_mainTalkPanel->GetScript<YouthText>()->ReturnTalker1(_nowText);
		ref->SetPanelImage(talker);
	}
}

bool EVENT_YOUTH_INTRO::SkipNextScene()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F3))
	{
		return true;
	}
	return false;

}
