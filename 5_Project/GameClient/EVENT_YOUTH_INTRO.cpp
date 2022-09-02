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
	// �ƺ��� �����Ѵ�. 
	shared_ptr<AdultBoyPrefab> adultBoyPrefab = make_shared<AdultBoyPrefab>(Vector3(0.7f, 0.f, -2.f));
	adultBoyPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	SceneManager::GetInstance()->GetActiveScene()->AddGameObject(adultBoyPrefab->gameObject);

	// ������ �����Ѵ�. 
	shared_ptr<AdultGirlPrefab> adultGirlPrefab = make_shared<AdultGirlPrefab>(Vector3(-0.7f, 0.f, -2.f));
	adultGirlPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	SceneManager::GetInstance()->GetActiveScene()->AddGameObject(adultGirlPrefab->gameObject);

	// ������ �����Ѵ�. 
	shared_ptr<DodoYouthPrefab> dodoYouthPrefab = make_shared<DodoYouthPrefab>(Vector3(-0.7f, 0.f, 2.f));
	dodoYouthPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	SceneManager::GetInstance()->GetActiveScene()->AddGameObject(dodoYouthPrefab->gameObject);

	// �踦 �����Ѵ�.
	shared_ptr<BoatPrefab> boatPrefab = make_shared<BoatPrefab>(Vector3(0.f, 3.f, -25.f));
	// ��ġ �巡���ϴ� ��ũ��Ʈ�� �ٸ��� ����..?
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

	// �÷��̾��� �ڽĿ� �踦 �־��ش�. 
	adultBoyPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
	adultGirlPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
	dodoYouthPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
	ref->_wave->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());

	ref->_boat = boatPrefab->gameObject;
	ref->_camera->GetTransform()->LookAt(Vector3(0, 0, 0));

	// ��ο� ä�� ����!
	ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(1.0f);
	_alpha = 1.0f;
	_isFade = true;
	ref->_fadeInOutPanel->GetComponent<Panel>()->SetIsAlpha(_isFade);

	// �̹��� ���� �ؽ�Ʈ
	ref->SetEventYouthNextText(0);
	ref->SetNextYouthtText(_nowText);

	// �÷��̾� ������ ����
	ref->isMove = false;
}

int EVENT_YOUTH_INTRO::Update()
{
	if (SkipNextScene())
	{
		return EventMachine::YOUTH_ROW;
	}

	// FadeIn ( ���� ��� ���� )
	if (_isFade)
	{
		_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		// FadeIn�� �����ٸ�..
		if (_alpha <= 0.01f)
		{
			// fade ��
			_isFade = false;

			// ���ϱ� ����
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

	// ��ȭ�� �����ٸ� true�� �ǰ� ������ Ʃ�丮�� �̺�Ʈ��..
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

	// �÷��̾� �ٽ� �����̰��ϱ�
	ref->isMove = true;
}

void EVENT_YOUTH_INTRO::IntroScript()
{
	if (_isTalk)
	{
		ref->_mainTalkPanel->SetActive(true);
		ref->_mainTalkPanel->GetScript<YouthText>()->_isPanelCheck = true;
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		// 3�ʸ��� ���ϴ���
		if (_talkTime > 3.0f)
		{
			_nowText++;
			ref->SetNextYouthtText(_nowText);
			_talkTime = 0.f;
		}

		// ���ϱⰡ �����ٸ�
		if (ref->_mainTalkPanel->GetScript<YouthText>()->Text1Size() == _nowText)
		{
			ref->_mainTalkPanel->GetScript<YouthText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

			_isTalk = false;

			ref->isYouthRow = true;			// ������ Ʃ�丮��� �̵�
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
