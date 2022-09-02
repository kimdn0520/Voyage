#include "pch.h"
#include "EVENT_YOUTH_SOLO.h"
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
#include "player.h"
#include "MoveObject.h"
#include "BottlePrefab.h"
#include "CameraScript.h"
#include "BoatPrefab.h"
#include "GameManager.h"
#include "YouthText.h"
#include <random>
#include "StarPrefab.h"
#include "BigStarPrefab.h"
#include "AdultBoyPrefab.h"
#include "AdultGirlPrefab.h"
#include "AdultPlayer.h"
#include "InputManager.h"


EVENT_YOUTH_SOLO::EVENT_YOUTH_SOLO(EventMachine* ref)
	: ref(ref)
{

}

EVENT_YOUTH_SOLO::~EVENT_YOUTH_SOLO()
{}

void EVENT_YOUTH_SOLO::Start()
{
	ref->SetNextYouthtText(0);
	ref->SetEventYouthNextText(3);
	ScriptCheck();

	ref->_mainTalkPanel->GetScript<YouthText>()->_isPanelCheck = true;
	ref->_mainTalkPanel->SetActive(true);

	_talkTime = 0.f;
	_isTalk = true;

	_isFade = true;
	_alpha = 1.f;
	_blinkTime = 2.f;

	// 앞에 보기 + 움직임 막기
	ref->_boat->GetTransform()->LookAt(Vector3(0.f, 3.f, -20.f));
	ref->isMove = false;
}

int EVENT_YOUTH_SOLO::Update()
{
	if (SkipNextScene())
	{
		// 다음에 로드할 씬이름을 Set해주고
		SceneManager::GetInstance()->SetLoadSceneName("AdulthoodScene");

		// 로딩씬으로 넘어간다.
		SceneManager::GetInstance()->LoadScene("LoadingScene");

		//return EventMachine::ADULTHOOD_INTRO;
	}

	// 페이드 인
	if (_isFade)
	{
		_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha <= 0.01f)
		{
			_alpha = 0.f;

			_isFade = false;
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}

	// 페이드 아웃 -> 성년기씬으로 넘어가기
	if (_isFadeOut)
	{
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / 4.0f;

		_boatObject->GetTransform()->SetLocalPosition(
			Vector3(_boatObject->GetTransform()->GetLocalPosition().x -20.f * TimeManager::GetInstance()->GetDeltaTime(),
			_boatObject->GetTransform()->GetLocalPosition().y, _boatObject->GetTransform()->GetLocalPosition().z));

		if (_alpha >= 1.f)
		{
			_alpha = 1.f;

			ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);

			_isFadeOut = false;
			
			// 다음에 로드할 씬이름을 Set해주고
			SceneManager::GetInstance()->SetLoadSceneName("AdulthoodScene");

			// 로딩씬으로 넘어간다.
			SceneManager::GetInstance()->LoadScene("LoadingScene");
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}

	IntroScript();

	if(ref->isAdulthoodIntro)
		return EventMachine::ADULTHOOD_INTRO;

	return EventMachine::YOUTH_SOLO;
}

void EVENT_YOUTH_SOLO::End()
{
	_boatObject.reset();
	ref->isMove = true;
}

void EVENT_YOUTH_SOLO::IntroScript()
{
	if (_isTalk)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		// 3초마다 말하는중
		if (_talkTime > 3.0f)
		{
			ScriptCheck();

			ref->SetNextYouthtText(_nowText);
			_talkTime = 0.f;
			_nowText++;
		}

		// 말이 끝나면 엄마아빠는 배타고 왼쪽으로간다.
		// 플레이어 베에있는 엄마 아빠 폭파
		if (ref->_mainTalkPanel->GetScript<YouthText>()->Text4Size() == _nowText)
		{
			hash<string> hasher;

			_isTalk = false;

			_isFadeOut = true;

			// 아빠를 생성한다. 
			shared_ptr<AdultBoyPrefab> adultBoyPrefab = make_shared<AdultBoyPrefab>(Vector3(0.7f, 0.f, 2.f));
			adultBoyPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
			adultBoyPrefab->gameObject->PlayAnim(adultBoyPrefab->gameObject, hasher("Rowing"), true);
			SceneManager::GetInstance()->GetActiveScene()->AddGameObject(adultBoyPrefab->gameObject);

			// 엄마를 생성한다. 
			shared_ptr<AdultGirlPrefab> adultGirlPrefab = make_shared<AdultGirlPrefab>(Vector3(-0.7f, 0.f, 2.f));
			adultGirlPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
			adultGirlPrefab->gameObject->PlayAnim(adultGirlPrefab->gameObject, hasher("Rowing.001"), true);
			SceneManager::GetInstance()->GetActiveScene()->AddGameObject(adultGirlPrefab->gameObject);

			// 배를 생성한다.
			shared_ptr<BoatPrefab> boatPrefab = make_shared<BoatPrefab>(Vector3(-20.f, 3.f, -30.f));
			SceneManager::GetInstance()->SetInstantiateGameObject(boatPrefab->gameObject);
			
			// 잠깐 끼어주기
			_boatObject = boatPrefab->gameObject;
			_boatObject->GetTransform()->LookAt(Vector3(-120.f, 3.f, -30.f));

			// 플레이어의 자식에 배를 넣어준다. 
			adultBoyPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
			adultGirlPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());

			// 플레이어 배의 엄마 아빠 오브젝트 삭제
			ref->_boat->GetScript<AdultPlayer>()->DestroyAdultBoyGirl();

			ref->_mainTalkPanel->GetScript<YouthText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);
		}
	}
}

void EVENT_YOUTH_SOLO::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<YouthText>()->ReturnTalker4(_nowText);
	ref->SetPanelImage(talker);
}

bool EVENT_YOUTH_SOLO::SkipNextScene()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F3))
	{
		return true;
	}
	return false;
}
