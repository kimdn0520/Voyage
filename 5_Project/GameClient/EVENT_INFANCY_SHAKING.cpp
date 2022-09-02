#include "pch.h"
#include "EVENT_INFANCY_SHAKING.h"
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
#include "GameManager.h"
#include "InputManager.h"

EVENT_INFANCY_SHAKING::EVENT_INFANCY_SHAKING(EventMachine* ref)
	: ref(ref)
{

}

EVENT_INFANCY_SHAKING::~EVENT_INFANCY_SHAKING()
{

}

// ���콺 ��ǲ �ȹް�
void EVENT_INFANCY_SHAKING::Start()
{
	_shakingCount = 0;
	_rotateCheck = 0;
	_isRight = true;
	_nowText = 0;
	ref->SetEventNextFatherText(2);
	ref->SetNextFatherText(_nowText);
	// �÷��̾� ���� ���
	ref->isMove = false;
	
	_isCheck1 = false;
	_isCheck2 = false;
	_isCheck3 = false;
	_isCheck4 = false;

	_isDeath = false;
	_alpha = 0.f;
}

int EVENT_INFANCY_SHAKING::Update()
{
	if (SkipNextEvent())
	{
		return EventMachine::INFANCY_END;

	}
	//ref->isInfacnyShaking = true;

	// �׾��ٸ�..!! fadeout �� �̺�Ʈ�� �ٽ� farming���� ���ư���.
	if (_isDeath)
	{
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		// FadeOut�� �����ٸ�..
		if (_alpha >= 1.0f)
		{
			ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(1.f);

			ref->_fadeInOutPanel->GetComponent<Panel>()->SetIsAlpha(true);
			
			ref->isShakingDeath = true;

			ref->_boat->GetScript<Player>()->ShakingEvent(false, true);

			// �Ĺ����� ����~
			return EventMachine::INFANCY_FARMING;
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}
	else
	{
		ShakingBoat();
		ref->_wave->activeSelf = ref->_boat->GetScript<Player>()->MoveCheck();
	}

	if (ref->isInfacnyShaking)
	{
		return EventMachine::INFANCY_END;
	}
	else
		return EventMachine::INFANCY_SHAKING;
}

void EVENT_INFANCY_SHAKING::End()
{
	ref->SetEventNextFatherText(3);
	_nowText = 0;
	ref->SetNextFatherText(_nowText);
	_shakingCount = 0;
	ref->isMove = true;

}

void EVENT_INFANCY_SHAKING::ShakingBoat()
{
	ScriptCheck();
	Talking();
	if (_shakingCount == 4)
	{
		_lastTalkTime += TimeManager::GetInstance()->GetDeltaTime();
		ref->_boat->GetScript<Player>()->ShakingEvent(false, _isRight);
		if (ref->_mainTalkPanel->GetScript<InfancyText>()->Text3Size() - 1 == _nowText)
		{
			if (_lastTalkTime > 3.0f)
			{
				ref->_mainTalkPanel->SetActive(false);
				ref->isInfacnyShaking = true;
				ref->_spacePanel->SetActive(false);

				return;
			}
		}
		if (_lastTalkTime > 3.0f && ref->_mainTalkPanel->GetScript<InfancyText>()->Text3Size() - 1 != _nowText)
		{
			_nowText++;
			ref->SetNextFatherText(_nowText);
			_lastTalkTime = 0.f;
			_talkTime = 0.f;
			ref->_mainTalkPanel->SetActive(true);
		}
		return;
	}
	_rotateTime += TimeManager::GetInstance()->GetDeltaTime();
	bool _check;
	// �踦 �������ش�. ifRight�� ������ üũ�ؼ� �־ �� ���� ��ŭ ��������
	// �׸��� �� ������ �޾ƿ�
	if (_isEventCheck)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime();
		if (_talkTime > 2.0f)
			ref->_mainTalkPanel->SetActive(false);

		if (_rotateTime > 0.03f)
		{
			_rotateCheck += ref->_boat->GetScript<Player>()->ShakingBoat(_isLifeCut, _isRight);
			_rotateTime = 0.f;
		}
		// ���� �������Ǹ� Life�� �����
		if (_rotateCheck >= 60 || _rotateCheck <= -60)
		{
			GameManager::GetInstance()->items[ItemType::Heart] -= 5;

			if (GameManager::GetInstance()->items[ItemType::Heart] <= 0)
			{
				GameManager::GetInstance()->items[ItemType::Heart] = 0;
				
				// �г� ������~
				ref->_mainTalkPanel->SetActive(false);
				
				// �׾��ٴ� �÷��� �����
				_isDeath = true;
			}

			_rotateCheck = 0;
			_isLifeCut = false;
			_isRight = !_isRight;
			_isCheck1 = false;
			_isCheck2 = false;
			_isCheck3 = false;
			_isCheck4 = false;
		}

		if(!_isDeath)
		{
			_check = ref->_boat->GetScript<Player>()->ShakingEvent(_isLifeCut, _isRight);
			if (_check)
			{
				_shakingCount++;
				_isRight = !_isRight;
			}
			else if (!_check && !_isLifeCut)
			{
				_isLifeCut = true;
				_isRight = !_isRight;
				_isEventCheck = false;
				_nowText = 0;
			}

			if (_shakingCount == 1 && !_isCheck1)
			{
				ref->_mainTalkPanel->SetActive(true);
				_nowText++;
				_isCheck1 = true;
				ref->SetNextFatherText(_nowText);
				_talkTime = 0.f;
			}
			else if (_shakingCount == 2 && !_isCheck2)
			{
				ref->_mainTalkPanel->SetActive(true);
				_nowText++;
				_isCheck2 = true;
				ref->SetNextFatherText(_nowText);
				_talkTime = 0.f;

			}
			else if (_shakingCount == 3 && !_isCheck3)
			{
				ref->_mainTalkPanel->SetActive(true);
				_nowText++;
				_isCheck3 = true;
				ref->SetNextFatherText(_nowText);
				_talkTime = 0.f;
			}
			else if (_shakingCount == 4 && !_isCheck4)
			{
				ref->_mainTalkPanel->SetActive(true);
				_nowText++;
				_isCheck4 = true;
				ref->SetNextFatherText(_nowText);
				_talkTime = 0.f;
			}
			else
				_rotateTime = 0.f;
		}

	}
}

void EVENT_INFANCY_SHAKING::Talking()
{
	// ��ȭâ�� ���ݴϴ�. 
	ref->_mainTalkPanel->SetActive(true);
	_talkTime += TimeManager::GetInstance()->GetDeltaTime();

	if (_talkTime > 2.0f)
	{
		if (_nowText >= 2)
		{
			ref->_mainTalkPanel->SetActive(false);
			//ref->isInfacnyShaking = true;
			_isEventCheck = true;
		}
		else
		{
			_nowText++;
			ref->SetNextFatherText(_nowText);
			_talkTime = 0.f;
		}
	}
	_rotateTime = 0.f;
}

void EVENT_INFANCY_SHAKING::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<InfancyText>()->ReturnTalker3(_nowText);
	ref->SetPanelImage(talker);
}


bool EVENT_INFANCY_SHAKING::SkipNextEvent()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F3))
	{
		return true;
	}
	return false;

}

void EVENT_INFANCY_SHAKING::SkipNextScene()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F4))
	{
		// ������ �ε��� ���̸��� Set���ְ�
		SceneManager::GetInstance()->SetLoadSceneName("YouthScene");

		// �ε������� �Ѿ��.
		SceneManager::GetInstance()->LoadScene("LoadingScene");
	}
}
