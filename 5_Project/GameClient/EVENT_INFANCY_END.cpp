#include "pch.h"
#include "EVENT_INFANCY_END.h"
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
#include "InfancyText.h"
#include "InputManager.h"
#include "GameManager.h"

EVENT_INFANCY_END::EVENT_INFANCY_END(EventMachine* ref)
	: ref(ref)
{

}

EVENT_INFANCY_END::~EVENT_INFANCY_END()
{

}

void EVENT_INFANCY_END::Start()
{
	_isCreate = true;
	ref->SetEventNextFatherText(3);
	_nowText = 0;
	ref->SetNextFatherText(_nowText);
	ref->isMove = false;
	ref->isFarming = false;

	_isTalk = true;
	ref->_mainTalkPanel->GetScript<InfancyText>()->_isPanelCheck = true;
	ref->_mainTalkPanel->SetActive(true);
}

int EVENT_INFANCY_END::Update()
{

	if (SkipNextEvent())
	{
		// ������ �ε��� ���̸��� Set���ְ�
		SceneManager::GetInstance()->SetLoadSceneName("YouthScene");

		// �ε������� �Ѿ��.
		SceneManager::GetInstance()->LoadScene("LoadingScene");
	}
	SkipNextScene();
	if (ref->_boat != nullptr)
		ref->_wave->activeSelf = ref->_boat->GetScript<Player>()->MoveCheck();

	// �� �Ұ��� �������� �����Ҷ� �ű⼭ �ٲ��.
	if (ref->isYouthIntro)
	{
		return EventMachine::YOUTH_INTRO;
	}

	// �ʿ��� ������Ʈ�� �����Ѵ�. 
	CreateBottle();


	// ����ȯ �� ���̵� �ƿ� - Talk�Լ����� _isFade = True �����ָ� �˴ϴ�. 
	if (_isFade)
	{
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha >= 1.0f)
		{
			_isFade = false;
			ref->_fadeInOutPanel->GetComponent<Panel>()->SetIsAlpha(_isFade);

			// ������ �ε��� ���̸��� Set���ְ�
			SceneManager::GetInstance()->SetLoadSceneName("YouthScene");

			// �ε������� �Ѿ��.
			SceneManager::GetInstance()->LoadScene("LoadingScene");
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}

	return EventMachine::INFANCY_END;
}

void EVENT_INFANCY_END::End()
{
	// ���� �������� ���� �����ش�. 
	if (bottlePrefab->gameObject != nullptr)
		SceneManager::GetInstance()->SetRemoveGameObject(bottlePrefab->gameObject);

	_isCreate = false;
	ref->isMove = true;
	ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(0.f);

	ref->_mainTalkPanel->GetScript<InfancyText>()->_isPanelCheck = false;
	ref->_mainTalkPanel->SetActive(false);
}

void EVENT_INFANCY_END::CreateBottle()
{
	Talk();
	if (!_isCreate)
	{
		bottlePrefab = make_shared<BottlePrefab>(Vector3(0.f, 1.5f, 0.f));
		SceneManager::GetInstance()->SetInstantiateGameObject(bottlePrefab->gameObject);
		_isCreate = !_isCreate;

		//_isFade = true;
	}
}

void EVENT_INFANCY_END::Talk()
{
	_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;
	/*ref->_mainTalkPanel->SetActive(true);
	if (_talkTime > 2.0f && _nowText < 2)
	{
		ref->SetNextFatherText(_nowText);
		ref->_mainTalkPanel->SetActive(false);
		_talkTime = 0.f;
		if (_nowText == 1)
		{
			_isCreate = false;
		}
		_nowText++;
	}
	else if (_talkTime > 5.0f && _isDoubleCheck)
	{
		_talkTime = 0.f;
		_nowText++;
		ref->_mainTalkPanel->SetActive(false);
		ref->SetNextFatherText(_nowText);
		if (ref->_mainTalkPanel->GetScript<InfancyText>()->Text4Size() - 1 == _nowText)
		{
			_isFade = true;
		}
	}*/

	if (_isTalk)
	{
		// 2�ʸ��� ���ϴ���
		if (_talkTime > 2.0f && _nowText < 2)
		{
			ScriptCheck();

			ref->SetNextFatherText(_nowText);
			
			if (_nowText == 1)
			{
				_isCreate = false;
			}
			
			_talkTime = 0.f;
			_nowText++;
		}
		else if (_talkTime > 5.0f && _nowText >= 2)
		{
			ScriptCheck();

			_talkTime = 0.f;
			ref->SetNextFatherText(_nowText);
			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<InfancyText>()->Text4Size() == _nowText)
		{
			_isTalk = false;

			_isFade = true;

			ref->_mainTalkPanel->SetActive(false);
		}
	}
}

void EVENT_INFANCY_END::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<InfancyText>()->ReturnTalker4(_nowText);
	ref->SetPanelImage(talker);
}

bool EVENT_INFANCY_END::SkipNextEvent()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F3))
	{
		return true;
	}
	return false;

}

void EVENT_INFANCY_END::SkipNextScene()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F4))
	{
		// ������ �ε��� ���̸��� Set���ְ�
		SceneManager::GetInstance()->SetLoadSceneName("YouthScene");

		// �ε������� �Ѿ��.
		SceneManager::GetInstance()->LoadScene("LoadingScene");
	}
}
