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
	// ���� �����Ѵ�. 
	_islandPrefab = make_shared<IslandPrefab>(Vector3(-50.f, 0.f, 150.f));
	//islandPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(4.f, 4.f, 4.f));
	SceneManager::GetInstance()->SetInstantiateGameObject(_islandPrefab->gameObject);

	// ���ű⸦ �����Ѵ�. 
	_birdPrefab = make_shared<BirdPrefab>(Vector3(1.f, -2.f, 2.f));
	_birdPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(2.f, 2.f, 2.f));
	_birdPrefab->gameObject->GetTransform()->SetLocalRotation(Vector3(30.f, 165.f, 0.f));
	SceneManager::GetInstance()->SetInstantiateGameObject(_birdPrefab->gameObject);

	// ���� �ڽ����� ���ű⸦ �ִ´�. 
	_birdPrefab->gameObject->GetComponent<Transform>()->SetParent(_islandPrefab->gameObject->GetTransform());

	// �̹��� ���� �ؽ�Ʈ
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

	// ���ű�� �浹�� �̺�Ʈ ����
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

	// ���ű�� �̾߱Ⱑ ������ -> ū �踦 ź�� -> ���̵� �ƿ� -> ���� �̺�Ʈ ��
	if (_isFade)
	{
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha >= 1.0f)
		{
			_isFade = false;

			// ���� ������ �̺�Ʈ true
			ref->isAdultCompetitor = true;

			_alpha = 1.0f;

			// ���ű�� ���� �ı��Ѵ�.
			SceneManager::GetInstance()->SetRemoveGameObject(_islandPrefab->gameObject);
			_islandPrefab.reset();
			SceneManager::GetInstance()->SetRemoveGameObject(_birdPrefab->gameObject);
			_birdPrefab.reset();
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}

	// ���� �̺�Ʈ�� ������ �̺�Ʈ��..
	if(ref->isAdultCompetitor)
		return EventMachine::ADULTHOOD_COMPETITOR;

	return EventMachine::ADULTHOOD_SEAGULL;
}

void EVENT_ADULTHOOD_SEAGULL::End()
{
	// �ٽ� ������ �� �ְ��ϰ�
	ref->isMove = true;

	// ���ǵ� �ٽ� false
	ref->isAdultCompetitor = false;
}

void EVENT_ADULTHOOD_SEAGULL::IntroScript()
{
	if (_isTalk)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		// 2�ʸ��� ���ϴ���
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

			// ���� �ִ� ��� ������ �����ؾ� �Ѵ�.
			SceneManager::GetInstance()->SetRemoveGameObject(ref->_boat);
			SceneManager::GetInstance()->SetRemoveGameObject(ref->_dodo);
			ref->_boat.reset();
			ref->_dodo.reset();

			// �踦 �����Ѵ�.
			shared_ptr<BigBoatPrefab> boatPrefab = make_shared<BigBoatPrefab>(Vector3(0.f, 3.f, -25.f));

			// ������ �����Ѵ�. 
			shared_ptr<DodoYouthPrefab> dodoYouthPrefab = make_shared<DodoYouthPrefab>(Vector3(-0.7f, 5.f, 1.5f));
			dodoYouthPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
			SceneManager::GetInstance()->SetInstantiateGameObject(dodoYouthPrefab->gameObject);

			// �踦 �������ش�.
			boatPrefab->gameObject->AddComponent<AdultPlayer>();
			boatPrefab->gameObject->GetScript<AdultPlayer>()->SetMainCamera(ref->_camera);
			boatPrefab->gameObject->GetScript<AdultPlayer>()->SetDodo(dodoYouthPrefab->gameObject);
			boatPrefab->gameObject->GetScript<AdultPlayer>()->SetFishingGaugeBack(boatPrefab->fishingGaugeBack);
			boatPrefab->gameObject->GetScript<AdultPlayer>()->SetFishingGauge(boatPrefab->fishingGauge);
			boatPrefab->gameObject->GetScript<AdultPlayer>()->SetFishingRange(boatPrefab->fishingRange);
			ref->_camera->GetScript<CameraScript>()->_targetTransfom = boatPrefab->gameObject->GetComponent<Transform>();
			SceneManager::GetInstance()->SetInstantiateGameObject(boatPrefab->gameObject);

			ref->_wave->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f,0.f,-12.8f));

			// ������ �θ�� �� ���� ���ش�. 
			dodoYouthPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
			ref->_wave->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());

			// �̺�Ʈ�ӽſ� ��Ʈ �־��ְ�~
			ref->_boat = boatPrefab->gameObject;

			// ī�޶� ��ġ ����
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
				// ī�޶� �÷��� 
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