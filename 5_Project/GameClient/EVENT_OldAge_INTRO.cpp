#include "pch.h"
#include <random>
#include "EVENT_OLDAGE_INTRO.h"
#include "EventMachine.h"
#include "IEventState.h"
#include "MoveObject.h"
#include "GameProcess.h"
#include "Transform.h"
#include "GameObject.h"
#include "Panel.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "CameraScript.h"
#include "BoatPrefab.h"
#include "DodoYouthPrefab.h"
#include "LilyFlowerPrefab.h"
#include "DataManager.h"
#include "AdultBoyPrefab.h"
#include "AdultGirlPrefab.h"
#include "OldAgeText.h"
#include "GameManager.h"
#include "AdultPlayer.h"
#include "BigBoatPrefab.h"
#include "Resources.h"
#include "Item.h"
#include "GrandFaPrefab.h"
#include "GrandMaPrefab.h"

EVENT_OLDAGE_INTRO::EVENT_OLDAGE_INTRO(EventMachine* ref) : ref(ref)
{}

EVENT_OLDAGE_INTRO::~EVENT_OLDAGE_INTRO()
{}

void EVENT_OLDAGE_INTRO::Start()
{
#pragma region Create
	hash<string> hasher;

	// �踦 �����Ѵ�.
	shared_ptr<BoatPrefab> boatPrefab = make_shared<BoatPrefab>(Vector3(0.f, 3.f, -30.f));

	// ������ �����Ѵ�. 
	shared_ptr<GrandFaPrefab> grandFaPrefab = make_shared<GrandFaPrefab>(Vector3(0.1f, 5.f, 1.2f));
	grandFaPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	grandFaPrefab->gameObject->PlayAnim(grandFaPrefab->gameObject, hasher("Rowing"), true);
	SceneManager::GetInstance()->SetInstantiateGameObject(grandFaPrefab->gameObject);

	// �ҸӴϸ� �����Ѵ�. 
	shared_ptr<GrandMaPrefab> grandMaPrefab = make_shared<GrandMaPrefab>(Vector3(-0.4f, 5.f, 1.5f));
	grandMaPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	SceneManager::GetInstance()->SetInstantiateGameObject(grandMaPrefab->gameObject);

	// �踦 �������ش�.
	boatPrefab->gameObject->AddComponent<AdultPlayer>();
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetMainCamera(ref->_camera);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetDodo(grandFaPrefab->gameObject);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetGrandMa(grandMaPrefab->gameObject);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetFishingGaugeBack(boatPrefab->fishingGaugeBack);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetFishingGauge(boatPrefab->fishingGauge);
	boatPrefab->gameObject->GetScript<AdultPlayer>()->SetFishingRange(boatPrefab->fishingRange);
	ref->_camera->GetScript<CameraScript>()->_targetTransfom = boatPrefab->gameObject->GetComponent<Transform>();
	SceneManager::GetInstance()->SetInstantiateGameObject(boatPrefab->gameObject);

	// ������ �θ�� �� ���� ���ش�. 
	grandFaPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
	grandMaPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());

	// �̺�Ʈ�ӽſ� ��Ʈ �־��ְ�~
	ref->_boat = boatPrefab->gameObject;

	// �̺�Ʈ�ӽſ� ������ �־��ش�.
	ref->_dodo = grandFaPrefab->gameObject;

	// �̺�Ʈ�ӽſ� �ҸӴϵ� �־��ش�.
	ref->_grandMa = grandMaPrefab->gameObject;

	// test��
	_isFade = true;
	_isTalk = true;

	_isEventCheck = true;
	ref->isFarming = false;
	
	ref->isMove = false;
	 
	_nowText = 0;
	ref->SetEventOldAgeNextText(0);
	ref->SetNextOldAgeText(_nowText);
}

int EVENT_OLDAGE_INTRO::Update()
{
	if (_isFade)
	{
		_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		// FadeIn�� �����ٸ�..
		if (_alpha <= 0.01f)
		{
			// fade ��
			_isFade = false;

			// �̺�Ʈ ����
			_isEventCheck = true;
			ref->isFarming = false;

			ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(0.f);
			//ref->_fadeInOutPanel->GetComponent<Panel>()->SetIsAlpha(_isFade);
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}
	if (_isEventCheck)
	{
		StreamFlower();
		ScriptCheck();
		Talk();
	}
	if (ref->isOldAgeFlishing)
		return EventMachine::OLDAGE_FLIASHING;
	return EventMachine::OLDAGE_INTRO;
}

void EVENT_OLDAGE_INTRO::End()
{
	ref->isOldAgeIntro = false;

	_nowText = 0;
	ref->SetEventOldAgeNextText(1);
	ref->SetNextOldAgeText(_nowText);
}

void EVENT_OLDAGE_INTRO::StreamFlower()
{
	_curSpawnTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	_curTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	if (_curSpawnTime > 2.f && _curFlowerCnt < 30)
	{
		// TODO : �����ϰ� ���վ����� ���� ������ ����������
		std::random_device rd;
		std::mt19937 gen(rd());
		uniform_int_distribution<int> dis(-90, 90);

		shared_ptr<LilyFlowerPrefab> lilyFlowerPrefab = make_shared<LilyFlowerPrefab>(Vector3(dis(gen), 0.f, 100.f));
		lilyFlowerPrefab->gameObject->AddComponent<Item>();
		lilyFlowerPrefab->gameObject->GetScript<Item>()->SetType(ItemType::LilyFlower);

		SceneManager::GetInstance()->SetInstantiateGameObject(lilyFlowerPrefab->gameObject);

		_curFlowerCnt++;
		_curSpawnTime = 0.f;
	}
}

void EVENT_OLDAGE_INTRO::Talk()
{
	if (_isTalk)
	{
		ref->_mainTalkPanel->SetActive(true);
		ref->_mainTalkPanel->GetScript<OldAgeText>()->_isPanelCheck = true;
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		if (_talkTime > 2.0f)
		{
			ref->SetNextOldAgeText(_nowText);
			_talkTime = 0.f;
			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<OldAgeText>()->Text1Size() == _nowText)
		{
			_isTalk = false;
			ref->_mainTalkPanel->GetScript<OldAgeText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

			hash<string> hasher;
			ref->_grandMa->PlayAnim(ref->_grandMa, hasher("Collapse"), false);
		}
	}
	else
	{
		ref->isOldAgeFlishing = true;
	}
}

void EVENT_OLDAGE_INTRO::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<OldAgeText>()->ReturnTalker1(_nowText);
	ref->SetPanelImage(talker);
}