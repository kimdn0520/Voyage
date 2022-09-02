#include "pch.h"
#include "EVENT_OLDAGE_FLIASHING.h"
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
#include "BigBoatPrefab.h"
#include "Resources.h"
#include "AdultPlayer.h"
#include <random>
#include "Item.h"

EVENT_OLDAGE_FLIASHING::EVENT_OLDAGE_FLIASHING(EventMachine* ref) : ref(ref)
{
}

EVENT_OLDAGE_FLIASHING::~EVENT_OLDAGE_FLIASHING()
{
}

void EVENT_OLDAGE_FLIASHING::Start()
{
	ref->_camera->GetScript<CameraScript>()->_targetTransfom = ref->_boat->GetComponent<Transform>();
	ref->_boat->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, -180.f, 0.f));
	ref->_dodo->GetTransform()->SetLocalPosition(Vector3(0.3f, 4.f, 1.2f));
	ref->_grandMa->GetTransform()->SetLocalPosition(Vector3(0.3f, 4.f, 1.2f));

	//테스트용
	_isFade = false;
	_isTalk = true;
	//_isEventCheck = true;

	// 움직임 멈추게 하자
	ref->isMove = false;
	_alpha = 0.f;

	// 이번에 나올 텍스트
	_nowText = 0;
	ref->SetEventOldAgeNextText(1);
	ref->SetNextOldAgeText(_nowText);
}

int EVENT_OLDAGE_FLIASHING::Update()
{
	if (ref->isOldAgeFlishing)
	{
		Talk();
		
		FlishingTalk();
		
		if (_isEventCheck)
		{
			CameraMove();
		}
	}

	if (_isFade)
	{
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		// FadeOut이 끝났다면..
		if (_alpha >= 1.f)
		{
			// fade 끝
			_isFade = false;

			_alpha = 1.f;

			// 다음에 로드할 씬이름을 Set해주고
			SceneManager::GetInstance()->SetLoadSceneName("HappyEndingScene");

			// 로딩씬으로 넘어간다.
			SceneManager::GetInstance()->LoadScene("HappyEndingScene");
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}

	return EventMachine::OLDAGE_FLIASHING;
}

void EVENT_OLDAGE_FLIASHING::End()
{
	ref->isOldAgeFlishing = false;

	ref->isOldAgeEnd = false;
}

void EVENT_OLDAGE_FLIASHING::CameraMove()
{
	// 카메라 이동해주자. 
	//  멀어지는 거를 카메라만 할까? 아님 오브젝트로 같이 할까?
	// 카메라 이동으로 해봤는데 이상해서 그냥 오브젝트 이동으로 변경 
	if (_boatCamerTransCameraTrans <= _boatCamerTrans)
	{
		ref->_boat->GetTransform()->MoveLook(1.5f * TimeManager::GetInstance()->GetDeltaTime());

		_totalCameraTime += 10.f * TimeManager::GetInstance()->GetDeltaTime();
		if (_totalCameraTime > 0.02f)
		{
			_totalCameraTime = 0.f;
		}
	}
	//if (_boatSumCamerRotateX <= _boatCamerRotateX)
	//{
	//	float rotatescale = 0.01f;
	//	_totalCameraTime += 10.f * TimeManager::GetInstance()->GetDeltaTime();
	//	if (_totalCameraTime > 0.02f)
	//	{
	//		// 카메라 올려줌 
	//		_boatSumCamerRotateX += rotatescale;
	//		ref->_camera->GetTransform()->RotateXAxis(-rotatescale);
	//		_totalCameraTime = 0.f;
	//	}
	//}
}


void EVENT_OLDAGE_FLIASHING::StreamFlower()
{
	_curSpawnTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	_curTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	if (_curSpawnTime > 2.f && _curFlowerCnt < 30)
	{
		// TODO : 랜덤하게 백합아이템 생성 서서히 내려오게함
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

void EVENT_OLDAGE_FLIASHING::Talk()
{
	if (_isTalk)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;


		// 2초마다 말하는중
		if (_talkTime > 3.0f)
		{
			ref->_mainTalkPanel->GetScript<OldAgeText>()->_isPanelCheck = true;
			ref->_mainTalkPanel->SetActive(true);

			ScriptCheck();

			ref->SetNextOldAgeText(_nowText);
			_talkTime = 0.f;
			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<OldAgeText>()->Text2Size() == _nowText)
		{
			_isTalk = false;

			_isFlisingTalk = true;

			_isEventCheck = true;

			_nowText = 0;

			ref->SetNextOldAgeText(_nowText);

			ref->SetEventOldAgeNextText(2);
		}

	}

}

void EVENT_OLDAGE_FLIASHING::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<OldAgeText>()->ReturnTalker2(_nowText);
	ref->SetPanelImage(talker);
}

void EVENT_OLDAGE_FLIASHING::FlishingTalk()
{
	if (_isFlisingTalk)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		if (_talkTime > 2.0f)
		{
			ref->_mainTalkPanel->GetScript<OldAgeText>()->_isPanelCheck = true;
			ref->_mainTalkPanel->SetActive(true);

			FliashingCheck();

			ref->SetNextOldAgeText(_nowText);
			_talkTime = 0.f;
			_nowText++;

		}
		if (ref->_mainTalkPanel->GetScript<OldAgeText>()->Text3Size() == _nowText)
		{
			ref->_mainTalkPanel->GetScript<OldAgeText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

			ref->_fadeInOutPanel->GetComponent<Panel>()->SetImages(L"black.png");
			ref->_fadeInOutPanel->GetComponent<Panel>()->SetIsAlpha(true);
			ref->_fadeInOutPanel->SetActive(true);

			_isFlisingTalk = false;

			_isFade = true;
		}
	}
}
void EVENT_OLDAGE_FLIASHING::FliashingCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<OldAgeText>()->ReturnTalker3(_nowText);
	ref->SetPanelImage(talker);
}