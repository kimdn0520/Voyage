#include "pch.h"
#include "EVENT_INTRO.h"
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
#include "BoatPrefab.h"
#include "MoveObject.h"
#include "AdultBoyPrefab.h"
#include "CameraScript.h"
#include "Player.h"
#include "InfancyText.h"
#include "AdultGirlPrefab.h"
#include "Particle.h"
#include "DataManager.h"
#include "Resources.h"
#include "TurtlePrefab.h"
#include "BabyPrefab.h"
#include "InputManager.h"
#include "GameManager.h"

EVENT_INTRO::EVENT_INTRO(EventMachine* ref)
	: ref(ref)
{}

EVENT_INTRO::~EVENT_INTRO()
{}

void EVENT_INTRO::Start()
{
	_introSumCamerRotateX = 0.f;
	_introStopTime = 0.f;
	_totalBoatTime = 0.f;
	_totalCameraTime = 0.f;
	_resetBoat = false;
	_isCreate = false;
	_isEndIntroTalk = false;
	_curBoatCnt = 0;
	_nowText = 0;
	ref->isMove = false;
	ref->SetEventNextFatherText(0);
	ref->SetNextFatherText(_nowText);
}

int EVENT_INTRO::Update()
{
	if(SkipNextEvent())
		return EventMachine::INFANCY_FARMING; 	// TODO : ���Ʊ� �Ĺ� �̺�Ʈ�� �Ѿ

	SkipNextScene();
	// TODO : �谡 �귯������ �ִϸ��̼�
	if (IntroAnimation())
	{
		if (_resetBoat)
			IntroCreateBoat();

		if (ref->_boat != nullptr)
			ref->_wave->activeSelf = ref->_boat->GetScript<Player>()->MoveCheck();

		// TODO : ī�޶��� ��ȯ�� �ʿ���
		if (IntroCameraAnimation())
		{
			//if (!_isEndIntroTalk)
			// TODO : �ִϸ��̼��� ������ ��� ��ũ��Ʈ�� �귯����
			ScriptCheck();
			IntroScript();
			if (ref->isFarming)
			{
				//ref->_isMainTalkOnOff = false;
				ref->_mainTalkPanel->GetScript<InfancyText>()->_isPanelCheck = false;
				ref->_mainTalkPanel->SetActive(false);
				return EventMachine::INFANCY_FARMING; 	// TODO : ���Ʊ� �Ĺ� �̺�Ʈ�� �Ѿ
			}
		}
	}
	return EventMachine::INTRO;
}

void EVENT_INTRO::End()
{
	// �� �̺�Ʈ�� ���������� �ѹ� ���� ���� ��

	ref->isIntro = false;
	_nowText = 0;
	_totalBoatTime = 0;
	ref->SetEventNextFatherText(1);
	ref->SetNextFatherText(_nowText);
}

bool EVENT_INTRO::IntroAnimation()
{
	_totalBoatTime += 1 * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	std::vector<Vector3> _pos;
	_pos.push_back(Vector3(-10.f, 3.f, -30.f));
	_pos.push_back(Vector3(50.f, 3.f, -30.f));
	_pos.push_back(Vector3(20.f, 3.f, 20.f));
	_pos.push_back(Vector3(-95.f, 3.f, 20.f));
	_pos.push_back(Vector3(-35.f, 3.f, 50.f));
	_pos.push_back(Vector3(100.f, 3.f, 35.f));

	if (_curBoatCnt < 6)
	{
		// �ִϸ��̼ǿ� �ʿ��� �踦 �����.
		for (int i = 0; i < 6; i++)
		{
			boatPrefab.push_back(make_shared<BoatPrefab>(_pos[i]));
			boatPrefab[i]->gameObject->AddComponent<MoveObject>();
			SceneManager::GetInstance()->SetInstantiateGameObject(boatPrefab[i]->gameObject);
			_curBoatCnt++;
		}
	}

	if (_totalBoatTime > 20.f)
	{
		return true;
	}
}

void EVENT_INTRO::IntroCreateBoat()
{
	if (!_isCreate)
	{
		// �ִϸ��̼� �ð��� ������ ������Ʈ�� �� �����Ѵ�. 
		for (auto boat : boatPrefab)
			SceneManager::GetInstance()->SetRemoveGameObject(boat->gameObject);
		// ��� ĳ���� ����


		// �ƺ��� �����Ѵ�. 
		shared_ptr<AdultBoyPrefab> adultBoyPrefab = make_shared<AdultBoyPrefab>(Vector3(0.7f, 0.f, 1.f));
		adultBoyPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
		SceneManager::GetInstance()->SetInstantiateGameObject(adultBoyPrefab->gameObject);

		// ������ �����Ѵ�. 
		shared_ptr<AdultGirlPrefab> adultGirlPrefab = make_shared<AdultGirlPrefab>(Vector3(-0.7f, 0.f, 1.f));
		adultGirlPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
		SceneManager::GetInstance()->SetInstantiateGameObject(adultGirlPrefab->gameObject);

		// �Ʊ⸦ �����Ѵ�. 
		shared_ptr<BabyPrefab> babyPrefab = make_shared<BabyPrefab>(Vector3(0.f, 0.f, 0.f));
		hash<string> hasher;
		babyPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
		babyPrefab->gameObject->PlayAnim(babyPrefab->gameObject, hasher("Sleep"), true);
		SceneManager::GetInstance()->SetInstantiateGameObject(babyPrefab->gameObject);

		// �踦 �����Ѵ�.
		shared_ptr<BoatPrefab> boatPrefab = make_shared<BoatPrefab>(Vector3(0.f, 3.f, -25.f));
		boatPrefab->gameObject->AddComponent<Player>();
		boatPrefab->gameObject->GetScript<Player>()->SetMainCamera(ref->_camera);
		boatPrefab->gameObject->GetScript<Player>()->SetAdultBoy(adultBoyPrefab->gameObject);
		boatPrefab->gameObject->GetScript<Player>()->SetAdultGirl(adultGirlPrefab->gameObject);
		boatPrefab->gameObject->GetScript<Player>()->SetBaby(babyPrefab->gameObject);
		boatPrefab->gameObject->GetScript<Player>()->SetFishingGaugeBack(boatPrefab->fishingGaugeBack);
		boatPrefab->gameObject->GetScript<Player>()->SetFishingGauge(boatPrefab->fishingGauge);
		boatPrefab->gameObject->GetScript<Player>()->SetFishingRange(boatPrefab->fishingRange);

		//shared_ptr<GameObject> particle = make_shared<GameObject>();
		//particle->AddComponent<Transform>();
		//particle->AddComponent<Particle>();
		//// ParticleInfo�� �����ϸ鼭 materialID�� �־��ش�. 
		///*particle->GetComponent<Particle>()->SetParticleInfo(DataManager::GetInstance()->GetParticleID());
		//Resources::GetInstance()->SetDiffuseMapTexture(particle->GetComponent<Particle>()->GetMaterialID(), L"wave.png");*/
		//SceneManager::GetInstance()->SetInstantiateGameObject(particle);

		ref->_camera->GetScript<CameraScript>()->_targetTransfom = boatPrefab->gameObject->GetComponent<Transform>();
		SceneManager::GetInstance()->SetInstantiateGameObject(boatPrefab->gameObject);

		// �÷��̾��� �ڽĿ� �踦 �־��ش�. 
		adultBoyPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
		adultGirlPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
		babyPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
		//particle->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
		ref->_wave->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());

		ref->_boat = boatPrefab->gameObject;

		_isCreate = true;
	}
}

bool EVENT_INTRO::IntroCameraAnimation()
{
	float rotatescale = 0.01f;
	_totalCameraTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	if (_introSumCamerRotateX <= _introCamerRotateX)
	{
		if (_totalCameraTime > 0.02f)
		{
			// ī�޶� �÷��� 
			_introSumCamerRotateX += rotatescale;
			ref->_camera->GetTransform()->RotateXAxis(-rotatescale);
			_totalCameraTime = 0.f;
		}
	}
	else
	{
		_resetBoat = true;
		_introStopTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;
		if (_introStopTime > 1.f)
		{
			if (1.55f <= ref->_camera->GetTransform()->GetWorldRotation().x)
			{
				ref->_camera->GetTransform()->LookAt(Vector3(0, 0, 0));
				return true;
			}
			if (_totalCameraTime > 0.02f)
			{
				// ī�޶� ������
				ref->_camera->GetTransform()->RotateXAxis(rotatescale);
				_totalCameraTime = 0.f;
			}
		}
	}
	return false;
}

void EVENT_INTRO::IntroScript()
{
	if (!_isEndIntroTalk)
	{
		ref->_mainTalkPanel->SetActive(true);
		ref->_mainTalkPanel->GetScript<InfancyText>()->_isPanelCheck = true;
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		if (_talkTime > 2.0f)
		{
			_nowText++;
			ref->SetNextFatherText(_nowText);
			_talkTime = 0.f;
		}
		// ��ǳ�� Ű �Է´�� �ð��� �޴´�. 
		/*if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::SPACE))
		{*/

		//	}

		if (ref->_mainTalkPanel->GetScript<InfancyText>()->Text1Size() == _nowText)
		{
			ref->_mainTalkPanel->GetScript<InfancyText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);
			_isEndIntroTalk = true;
 			ref->isMove = true;
		}

		ref->isFarming = false;
	}
	else
		ref->isFarming = true;
}

void EVENT_INTRO::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<InfancyText>()->ReturnTalker1(_nowText);
	ref->SetPanelImage(talker);
}

bool EVENT_INTRO::SkipNextEvent()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F3))
	{
		return true;
	}
	return false;
}

void EVENT_INTRO::SkipNextScene()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F4))
	{
		// ������ �ε��� ���̸��� Set���ְ�
		SceneManager::GetInstance()->SetLoadSceneName("YouthScene");

		// �ε������� �Ѿ��.
		SceneManager::GetInstance()->LoadScene("LoadingScene");
	}
}
