#include "pch.h"
#include "EVENT_ADULTHOOD_COMPETITOR.h"
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
#include "SphereCollider.h"
#include "CompetitorController.h"
#include "Button.h"
#include "CompetitorTrace.h"
#include "Text.h"
#include "NewbieEngineFunc.h"

EVENT_ADULTHOOD_COMPETITOR::EVENT_ADULTHOOD_COMPETITOR(EventMachine* ref)
	: ref(ref)
{

}

EVENT_ADULTHOOD_COMPETITOR::~EVENT_ADULTHOOD_COMPETITOR()
{}

void EVENT_ADULTHOOD_COMPETITOR::Start()
{
	// 이번에 나올 텍스트
	_nowText = 0;
	ref->SetEventAdultNextText(2);
	ref->SetNextAdultText(_nowText);

	_curTime = 0.f;
	_blinkTime = 3.f;
	_alpha = 1.f;
	_isFade = true;
	_isTalk = true;
	_isSpawn = true;

	// 콜라이더 크기좀 줄여주자
	ref->_boat->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, -1.f), 14.f);

	// 시작하자마자 경쟁자 이상한쪽으로 움직여서 해줬음
	GameManager::GetInstance()->_moveToPoint = Vector3(0.f, 0.f, 0.f);
}

int EVENT_ADULTHOOD_COMPETITOR::Update()
{
	// 갈매기 등장 시간 10초(대화도 출력) / 경쟁자 이벤트 시간 40초
	_curTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	if (_isFade)
	{
		_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha <= 0.01f)
		{
			_isFade = false;

			_alpha = 0.f;

			_isCompetitor = true;

			ref->isMove = true;
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}

	// Fade가 끝나고 2초뒤 경쟁자 이벤트 시작
	if (_isCompetitor && _curTime >= 2.f)
	{
		// 경쟁자를 스폰안했다면 스폰해준다.
		if (_isSpawn)
		{
			SpawnCompetitor();
		}

		if (_curTime >= 10.f && _curTime <= 11.f)  
		{
			ref->_competitionPanel1->SetActive(false);
			ref->_competitionPanel2->SetActive(false);
		}

		// 10초 뒤 스크립트 진행
		if (_curTime >= 10.f)
			IntroScript();

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
	}

	if(_curTime >= 40.f)
		return EventMachine::ADULTHOOD_SWIRL;

	if (GameManager::GetInstance()->items[ItemType::Heart] == 0)
	{
		ref->isMove = false;

		/*SceneManager::GetInstance()->SetRemoveGameObject(competitorPanel1);
		SceneManager::GetInstance()->SetRemoveGameObject(competitorPanel2);*/
		SceneManager::GetInstance()->SetRemoveGameObject(boatPrefab1->gameObject);
		SceneManager::GetInstance()->SetRemoveGameObject(boatPrefab2->gameObject);
		SceneManager::GetInstance()->SetRemoveGameObject(boatPrefab3->gameObject);
		SceneManager::GetInstance()->SetRemoveGameObject(competitorPrefab1->gameObject);
		SceneManager::GetInstance()->SetRemoveGameObject(competitorPrefab2->gameObject);
		SceneManager::GetInstance()->SetRemoveGameObject(competitorPrefab3->gameObject);
		/*competitorPanel1.reset();
		competitorPanel2.reset();*/
		boatPrefab1->gameObject.reset();
		boatPrefab2->gameObject.reset();
		boatPrefab3->gameObject.reset();
		boatPrefab1.reset();
		boatPrefab2.reset();
		boatPrefab3.reset();
		competitorPrefab1->gameObject.reset();
		competitorPrefab2->gameObject.reset();
		competitorPrefab3->gameObject.reset();
		competitorPrefab1.reset();
		competitorPrefab2.reset();
		competitorPrefab3.reset();

		// 이번에 나올 텍스트
		_nowText = 0;
		ref->SetEventAdultNextText(2);
		ref->SetNextAdultText(_nowText);

		_curTime = 0.f;
		_blinkTime = 2.f;
		_alpha = 1.f;
		_isFade = true;
		_isTalk = true;
		_isSpawn = true;

		// 콜라이더 크기좀 줄여주자
		ref->_boat->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, -1.f), 14.f);

		// 시작하자마자 경쟁자 이상한쪽으로 움직여서 해줬음
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
	}
	

	return EventMachine::ADULTHOOD_COMPETITOR;
}

void EVENT_ADULTHOOD_COMPETITOR::End()
{
	ref->isSelectedYes = false;
	ref->isSelectedNo = false;

	ref->_competitionPanel1.reset();
	ref->_competitionPanel2.reset();

	boatPrefab1->gameObject.reset();
	boatPrefab2->gameObject.reset();
	boatPrefab3->gameObject.reset();
	boatPrefab1.reset();
	boatPrefab2.reset();
	boatPrefab3.reset();

	competitorPrefab1->gameObject.reset();
	competitorPrefab2->gameObject.reset();
	competitorPrefab3->gameObject.reset();
	competitorPrefab1.reset();
	competitorPrefab2.reset();
	competitorPrefab3.reset();
}

void EVENT_ADULTHOOD_COMPETITOR::IntroScript()
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

		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text3Size() == _nowText)
		{
			ref->SetPanelImage((int)IDNUM::SEAGULL_YES_NO);

			_isTalk = false;

			_isChoiceEvent = true;

			// 잠들어있던.. yes, no 버튼을 깨워준다.
			ref->_yesButton->SetActive(true);
			ref->_noButton->SetActive(true);

			// 시간을.. 멈춘다..!?
			SceneManager::GetInstance()->isPause = true;
			_curTime = 25.f;
		}
	}
}

void EVENT_ADULTHOOD_COMPETITOR::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker3(_nowText);
	ref->SetPanelImage(talker);
}

void EVENT_ADULTHOOD_COMPETITOR::SpawnCompetitor()
{
	hash<string> hasher;

	// 배를 생성한다.
	boatPrefab1 = make_shared<BoatPrefab>(Vector3(-80.f, 3.f, -30.f));
	boatPrefab1->gameObject->SetTag(Tag::Enemy);
	boatPrefab1->gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, -1.f), 14.f);
	boatPrefab1->gameObject->AddComponent<CompetitorController>();
	SceneManager::GetInstance()->SetInstantiateGameObject(boatPrefab1->gameObject);
	
	// 경쟁자를 생성한다.
	competitorPrefab1 = make_shared<AdultBoyPrefab>(Vector3(0.f, 0.f, 1.5f));
	competitorPrefab1->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	competitorPrefab1->gameObject->PlayAnim(competitorPrefab1->gameObject, hasher("Rowing"), true);
	SceneManager::GetInstance()->SetInstantiateGameObject(competitorPrefab1->gameObject);

	// 경쟁자의 부모로 배 설정 해준다. 
	competitorPrefab1->gameObject->GetComponent<Transform>()->SetParent(boatPrefab1->gameObject->GetTransform());
	boatPrefab1->gameObject->SetChild(competitorPrefab1->gameObject);
	

	// 배를 생성한다.
	boatPrefab2 = make_shared<BoatPrefab>(Vector3(40.f, 3.f, -30.f));
	boatPrefab2->gameObject->SetTag(Tag::Enemy);
	boatPrefab2->gameObject->AddComponent<CompetitorController>();
	boatPrefab2->gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, -1.f), 14.f);
	SceneManager::GetInstance()->SetInstantiateGameObject(boatPrefab2->gameObject);

	// 경쟁자를 생성한다.
	competitorPrefab2 = make_shared<AdultBoyPrefab>(Vector3(0.f, 0.f, 1.5f));
	competitorPrefab2->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	competitorPrefab2->gameObject->PlayAnim(competitorPrefab2->gameObject, hasher("Rowing"), true);
	SceneManager::GetInstance()->SetInstantiateGameObject(competitorPrefab2->gameObject);

	// 경쟁자의 부모로 배 설정 해준다. 
	competitorPrefab2->gameObject->GetComponent<Transform>()->SetParent(boatPrefab2->gameObject->GetTransform());
	boatPrefab2->gameObject->SetChild(competitorPrefab2->gameObject);

	// 배를 생성한다.
	boatPrefab3 = make_shared<BoatPrefab>(Vector3(-30.f, 3.f, -75.f));
	boatPrefab3->gameObject->SetTag(Tag::Enemy);
	boatPrefab3->gameObject->AddComponent<CompetitorTrace>();
	boatPrefab3->gameObject->GetScript<CompetitorTrace>()->SetPlayer(ref->_boat);
	boatPrefab3->gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, -1.f), 14.f);
	SceneManager::GetInstance()->SetInstantiateGameObject(boatPrefab3->gameObject);

	// 경쟁자를 생성한다.
	competitorPrefab3 = make_shared<AdultBoyPrefab>(Vector3(0.f, 0.f, 1.5f));
	competitorPrefab3->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	competitorPrefab3->gameObject->PlayAnim(competitorPrefab3->gameObject, hasher("Rowing"), true);
	SceneManager::GetInstance()->SetInstantiateGameObject(competitorPrefab3->gameObject);

	// 경쟁자의 부모로 배 설정 해준다. 
	competitorPrefab3->gameObject->GetComponent<Transform>()->SetParent(boatPrefab3->gameObject->GetTransform());
	boatPrefab3->gameObject->SetChild(competitorPrefab3->gameObject);

	//// 경쟁자 대사
	//competitorPanel1 = make_shared<GameObject>();
	//competitorPanel1->SetName("competitorPanel1");
	//competitorPanel1->AddComponent<Transform>();
	//competitorPanel1->AddComponent<Panel>();
	//competitorPanel1->GetComponent<Panel>()->SetOffset(
	//	GameProcess::_windowInfo.width / 2 - 850.f,
	//	GameProcess::_windowInfo.width / 2 - 400.f,
	//	GameProcess::_windowInfo.height / 2 - 200.f,
	//	GameProcess::_windowInfo.height / 2 + 200.f);
	//competitorPanel1->GetComponent<Panel>()->SetImages(L"speech bubble3.png");
	//competitorPanel1->GetComponent<Panel>()->SetSortLayer(0);
	//competitorPanel1->AddComponent<Text>();
	//competitorPanel1->GetComponent<Text>()->SetLocation(200, 500, GameProcess::_windowInfo.width, GameProcess::_windowInfo.height);
	//competitorPanel1->GetComponent<Text>()->GetTextInfo()->text = "바쁘다 바빠!";
	//competitorPanel1->GetComponent<Text>()->GetTextInfo()->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	//competitorPanel1->GetComponent<Text>()->GetTextInfo()->scale = 2.f;
	//competitorPanel1->SetActive(false);
	//SceneManager::GetInstance()->SetInstantiateGameObject(competitorPanel1);

	//// 경쟁자 대사
	//competitorPanel2 = make_shared<GameObject>();
	//competitorPanel2->SetName("competitorPanel2");
	//competitorPanel2->AddComponent<Transform>();
	//competitorPanel2->AddComponent<Panel>();
	//competitorPanel2->GetComponent<Panel>()->SetOffset(
	//	GameProcess::_windowInfo.width / 2 + 400.f,
	//	GameProcess::_windowInfo.width / 2 + 800.f,
	//	GameProcess::_windowInfo.height / 2 - 200.f,
	//	GameProcess::_windowInfo.height / 2 + 200.f);
	//competitorPanel2->GetComponent<Panel>()->SetImages(L"speech bubble3.png");
	//competitorPanel2->GetComponent<Panel>()->SetSortLayer(0);
	//competitorPanel2->AddComponent<Text>();
	//competitorPanel2->GetComponent<Text>()->SetLocation(1450, 500, GameProcess::_windowInfo.width, GameProcess::_windowInfo.height);
	//competitorPanel2->GetComponent<Text>()->GetTextInfo()->text = "비켜 비켜!!";
	//competitorPanel2->GetComponent<Text>()->GetTextInfo()->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	//competitorPanel2->GetComponent<Text>()->GetTextInfo()->scale = 2.f;
	//competitorPanel2->SetActive(false);
	//SceneManager::GetInstance()->SetInstantiateGameObject(competitorPanel2);

	ref->_competitionPanel1->SetActive(true);
	ref->_competitionPanel2->SetActive(true);

	//ResizeWindow(GameProcess::resizeInfo.width, GameProcess::resizeInfo.height);

	_isSpawn = false;
}

void EVENT_ADULTHOOD_COMPETITOR::YesScript()
{
	if (_isTalk_Y)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		// 2초마다 말하는중
		if (_talkTime > 2.0f)
		{
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
			ref->_mainTalkPanel->SetActive(true);

			YesScriptCheck();

			ref->SetNextAdultText(_nowText);

			// 체력 max !
			if(_nowText == 1)
				GameManager::GetInstance()->items[ItemType::Heart] = 30;

			_talkTime = 0.f;
			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text11Size() == _nowText)
		{
			_isTalk_Y = false;

			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

			_isChoiceEvent = true;
		}
	}
}

void EVENT_ADULTHOOD_COMPETITOR::YesScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker11(_nowText);
	ref->SetPanelImage(talker);
}

void EVENT_ADULTHOOD_COMPETITOR::NoScript()
{
	if (_isTalk_N)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

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

		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text12Size() == _nowText)
		{
			_isTalk_N = false;

			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

			_isChoiceEvent = true;
		}
	}
}

void EVENT_ADULTHOOD_COMPETITOR::NoScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker12(_nowText);
	ref->SetPanelImage(talker);
}
