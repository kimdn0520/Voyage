#include "pch.h"
#include "EVENT_YOUTH_ROW.h"
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
#include "AdultPlayer.h"
#include "InputManager.h"

EVENT_YOUTH_ROW::EVENT_YOUTH_ROW(EventMachine* ref)
	: ref(ref)
{

}

EVENT_YOUTH_ROW::~EVENT_YOUTH_ROW()
{}

void EVENT_YOUTH_ROW::Start()
{
	ref->SetNextYouthtText(0);
	ref->SetEventYouthNextText(1);
	GameManager::GetInstance()->isPlayerStop = true;
	ref->_mainTalkPanel->GetScript<YouthText>()->_isPanelCheck = true;
	ref->_mainTalkPanel->SetActive(true);
	ref->_arrowPanel->SetActive(true);
	_isTalk = true;
	_isTalkEvent = true;
}

int EVENT_YOUTH_ROW::Update()
{
	if (SkipNextScene())
		return EventMachine::YOUTH_FARMING;

	if (ref->isFarming)
		return EventMachine::YOUTH_FARMING;

	ScriptCheck();
	IntroScript();

	return EventMachine::YOUTH_ROW;
}

void EVENT_YOUTH_ROW::End()
{
	_isTalk = false;
	_isTalkEvent = false;
	ref->_arrowPanel->SetActive(false);

}

void EVENT_YOUTH_ROW::IntroScript()
{
	if (ref->_boat != nullptr)
		ref->_wave->activeSelf = ref->_boat->GetScript<AdultPlayer>()->MoveCheck();

	if (_isTalk)
	{
		if (_isTalkEvent)
		{
			ref->SetNextYouthtText(_nowText);

			_isTalkEvent = false;
		}

		// 노젓기에 따라 나온다.
		if (GameManager::GetInstance()->isRow)
		{
			_isTalkEvent = true;

			GameManager::GetInstance()->isRow = false;
			ref->_arrowPanel->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, -35.f, 0.f));

			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<YouthText>()->Text2Size() == _nowText)
		{
			_isTalk = false;

			ref->isFarming = true;
		}
	}
}

void EVENT_YOUTH_ROW::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<YouthText>()->ReturnTalker2(_nowText);
	ref->SetPanelImage(talker);
}

bool EVENT_YOUTH_ROW::SkipNextScene()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F3))
	{
		return true;
	}
	return false;

}
