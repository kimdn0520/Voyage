#include "pch.h"
#include <random>
#include "EVENT_ADULTHOOD_SWIRL.h"
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
#include "SwirlPrefab.h"
#include "SphereCollider.h"

EVENT_ADULTHOOD_SWIRL::EVENT_ADULTHOOD_SWIRL(EventMachine* ref)
	: ref(ref)
{

}

EVENT_ADULTHOOD_SWIRL::~EVENT_ADULTHOOD_SWIRL()
{}

void EVENT_ADULTHOOD_SWIRL::Start()
{
	// 이번에 나올 텍스트
	_nowText = 0;
	ref->SetEventAdultNextText(3);
	ref->SetNextAdultText(_nowText);

	_blinkTime = 3.f;
	_isTalk = true;
	_isSpawn = true;
}

int EVENT_ADULTHOOD_SWIRL::Update()
{
	if (ref->_boat != nullptr)
		ref->_wave->activeSelf = ref->_boat->GetScript<AdultPlayer>()->MoveCheck();

	if (_isSpawn)
	{
		SpawnSwirl();
	}

	// 꿈을 안판 상태였다면 나오는 텍스트
	if (!ref->isSellDream)
	{
		IntroScript();
	}

	// yes를 눌렀을 경우
	if (ref->isSelectedYes)
	{
		if (!_isSetTextInfo)
		{
			// 버튼들은 꺼준다.
			ref->_yesButton->SetActive(false);
			ref->_noButton->SetActive(false);

			ref->SetEventAdultNextText(9);
			_nowText = 0;
			ref->SetNextAdultText(_nowText);

			// 갈매기 한테 꿈을 팔았다!
			ref->isSellDream = true;

			// 정보를 set 했다
			_isSetTextInfo = true;

			// talk_y 시작
			_isTalk_Y = true;

			YesScriptCheck();
		}

		YesScript();
	}

	// no를 눌렀을 경우
	if (ref->isSelectedNo)
	{
		if (!_isSetTextInfo)
		{
			// 버튼들은 꺼준다.
			ref->_yesButton->SetActive(false);
			ref->_noButton->SetActive(false);

			ref->SetEventAdultNextText(10);
			_nowText = 0;
			ref->SetNextAdultText(_nowText);

			// 정보를 set 했다
			_isSetTextInfo = true;

			// talk_n 시작
			_isTalk_N = true;

			NoScriptCheck();
		}

		NoScript();
	}

	if (GameManager::GetInstance()->items[ItemType::Heart] == 0)
	{
		// 플레이어 움직여
		ref->isMove = true;

		// 다시 소용돌이 스폰해
		_isSpawn = true;

		// 이번에 나올 텍스트
		_isTalk = true;

		ref->SetEventAdultNextText(3);
		_nowText = 0;
		ref->SetNextAdultText(_nowText);

		_curTime = 0.f;
		_blinkTime = 2.f;
		_alpha = 1.f;
		_isFade = true;

		_curSpawnTime = 0.f;

		GameManager::GetInstance()->_moveToPoint = Vector3(0.f, 0.f, 0.f);

		GameManager::GetInstance()->items[ItemType::Heart] = 5;

		// 패널 꺼준다.
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
		ref->_mainTalkPanel->SetActive(false);

		ref->isSelectedNo = false;
		ref->isSelectedYes = false;
		_isSetTextInfo = false;

		ref->_yesButton->SetActive(false);
		ref->_noButton->SetActive(false);

		SceneManager::GetInstance()->SetRemoveGameObject(ref->_boat->GetScript<AdultPlayer>()->disaster);
	}

	if (ref->isAdultSwirl)
		return EventMachine::ADULTHOOD_THUNDER;

	return EventMachine::ADULTHOOD_SWIRL;
}

void EVENT_ADULTHOOD_SWIRL::End()
{
	_curSwirlCnt = 0;
}

void EVENT_ADULTHOOD_SWIRL::IntroScript()
{
	if (_isTalk)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime();

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

		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text4Size() == _nowText)
		{
			ref->SetPanelImage((int)IDNUM::SEAGULL_YES_NO);

			_isTalk = false;

			// 잠들어있던.. yes, no 버튼을 깨워준다.
			ref->_yesButton->SetActive(true);
			ref->_noButton->SetActive(true);

			// 시간을.. 멈춘다..!?
			SceneManager::GetInstance()->isPause = true;
			
			// 한.. 3부터?
			_curSwirlCnt = 3;
		}
	}
}

void EVENT_ADULTHOOD_SWIRL::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker4(_nowText);
	ref->SetPanelImage(talker);
}

void EVENT_ADULTHOOD_SWIRL::SpawnSwirl()
{	
	// 꿈을 팔았다면 작은 소용돌이 조금 나옴
	if (ref->isSellDream)
	{
		_curSpawnTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		if (_curSpawnTime > 2.f && _curSwirlCnt < 10)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			uniform_int_distribution<int> dis(-90, 90);

			shared_ptr<SwirlPrefab> swirlPrefab = make_shared<SwirlPrefab>(Vector3(dis(gen), -2.f, 100.f));
			swirlPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(5.f, 5.f, 5.f));
			swirlPrefab->gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 10.f, 0.f), 5.f);
			SceneManager::GetInstance()->SetInstantiateGameObject(swirlPrefab->gameObject);

			_curSwirlCnt++;
			_curSpawnTime = 0.f;
		}

		if (_curSwirlCnt >= 10)
		{
			ref->isAdultSwirl = true;
			_isSpawn = false;
		}
	}
	// 꿈을 안팔았다면 거대한 소용돌이 30개 .. 대사도 쳐아함
	else
	{
		_curSpawnTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;;

		if (_curSpawnTime > 2.f && _curSwirlCnt < 30)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			uniform_int_distribution<int> dis(-90, 90);

			shared_ptr<SwirlPrefab> swirlPrefab = make_shared<SwirlPrefab>(Vector3(dis(gen), -7.f, 100.f));
			swirlPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(20.f, 20.f, 20.f));
			SceneManager::GetInstance()->SetInstantiateGameObject(swirlPrefab->gameObject);

			_curSwirlCnt++;
			_curSpawnTime = 0.f;
		}

		if (_curSwirlCnt >= 30)
		{
			ref->isAdultSwirl = true;
			_isSpawn = false;
		}
	}
}

void EVENT_ADULTHOOD_SWIRL::YesScript()
{
	if (_isTalk_Y)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime();

		// 2초마다 말하는중
		if (_talkTime > 2.0f)
		{
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
			ref->_mainTalkPanel->SetActive(true);

			YesScriptCheck();

			ref->SetNextAdultText(_nowText);

			_talkTime = 0.f;
			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text13Size() == _nowText)
		{
			_isTalk_Y = false;

			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

		}
	}
}

void EVENT_ADULTHOOD_SWIRL::YesScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker13(_nowText);
	ref->SetPanelImage(talker);
}

void EVENT_ADULTHOOD_SWIRL::NoScript()
{
	if (_isTalk_N)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime();

		// 2초마다 말하는중
		if (_talkTime > 2.0f)
		{
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
			ref->_mainTalkPanel->SetActive(true);

			NoScriptCheck();

			ref->SetNextAdultText(_nowText);
			_talkTime = 0.f;
			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text14Size() == _nowText)
		{
			_isTalk_N = false;

			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

		}
	}
}

void EVENT_ADULTHOOD_SWIRL::NoScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker14(_nowText);
	ref->SetPanelImage(talker);
}