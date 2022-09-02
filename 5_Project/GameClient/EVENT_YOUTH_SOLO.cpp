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

	// �տ� ���� + ������ ����
	ref->_boat->GetTransform()->LookAt(Vector3(0.f, 3.f, -20.f));
	ref->isMove = false;
}

int EVENT_YOUTH_SOLO::Update()
{
	if (SkipNextScene())
	{
		// ������ �ε��� ���̸��� Set���ְ�
		SceneManager::GetInstance()->SetLoadSceneName("AdulthoodScene");

		// �ε������� �Ѿ��.
		SceneManager::GetInstance()->LoadScene("LoadingScene");

		//return EventMachine::ADULTHOOD_INTRO;
	}

	// ���̵� ��
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

	// ���̵� �ƿ� -> ���������� �Ѿ��
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
			
			// ������ �ε��� ���̸��� Set���ְ�
			SceneManager::GetInstance()->SetLoadSceneName("AdulthoodScene");

			// �ε������� �Ѿ��.
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

		// 3�ʸ��� ���ϴ���
		if (_talkTime > 3.0f)
		{
			ScriptCheck();

			ref->SetNextYouthtText(_nowText);
			_talkTime = 0.f;
			_nowText++;
		}

		// ���� ������ �����ƺ��� ��Ÿ�� �������ΰ���.
		// �÷��̾� �����ִ� ���� �ƺ� ����
		if (ref->_mainTalkPanel->GetScript<YouthText>()->Text4Size() == _nowText)
		{
			hash<string> hasher;

			_isTalk = false;

			_isFadeOut = true;

			// �ƺ��� �����Ѵ�. 
			shared_ptr<AdultBoyPrefab> adultBoyPrefab = make_shared<AdultBoyPrefab>(Vector3(0.7f, 0.f, 2.f));
			adultBoyPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
			adultBoyPrefab->gameObject->PlayAnim(adultBoyPrefab->gameObject, hasher("Rowing"), true);
			SceneManager::GetInstance()->GetActiveScene()->AddGameObject(adultBoyPrefab->gameObject);

			// ������ �����Ѵ�. 
			shared_ptr<AdultGirlPrefab> adultGirlPrefab = make_shared<AdultGirlPrefab>(Vector3(-0.7f, 0.f, 2.f));
			adultGirlPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
			adultGirlPrefab->gameObject->PlayAnim(adultGirlPrefab->gameObject, hasher("Rowing.001"), true);
			SceneManager::GetInstance()->GetActiveScene()->AddGameObject(adultGirlPrefab->gameObject);

			// �踦 �����Ѵ�.
			shared_ptr<BoatPrefab> boatPrefab = make_shared<BoatPrefab>(Vector3(-20.f, 3.f, -30.f));
			SceneManager::GetInstance()->SetInstantiateGameObject(boatPrefab->gameObject);
			
			// ��� �����ֱ�
			_boatObject = boatPrefab->gameObject;
			_boatObject->GetTransform()->LookAt(Vector3(-120.f, 3.f, -30.f));

			// �÷��̾��� �ڽĿ� �踦 �־��ش�. 
			adultBoyPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());
			adultGirlPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());

			// �÷��̾� ���� ���� �ƺ� ������Ʈ ����
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
