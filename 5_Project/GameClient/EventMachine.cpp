#include "pch.h"
#include "EventMachine.h"
#include "IEventState.h"
#include "EVENT_TITLE.h"
#include "EVENT_INTRO.h"
#include "EVENT_INFANCY_FARMING.h"
#include "EVENT_INFANCY_SHAKING.h"
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
#include "CameraScript.h"
#include "Player.h"
#include "EVENT_INFANCY_END.h"
#include "EVENT_YOUTH_INTRO.h"
#include "EVENT_YOUTH_ROW.h"
#include "EVENT_ADULTHOOD_INTRO.h"
#include "EVENT_YOUTH_FARMING.h"
#include "EVENT_YOUTH_SOLO.h"
#include "Text.h"
#include "YouthText.h"
#include "AdulthoodText.h"
#include "OldAgeText.h"
#include "EVENT_ADULTHOOD_SEAGULL.h"
#include "EVENT_ADULTHOOD_COMPETITOR.h"
#include "EVENT_ADULTHOOD_SWIRL.h"
#include "EVENT_OLDAGE_INTRO.h"
#include "EVENT_OLDAGE_FLIASHING.h"
#include "EVENT_OLDAGE_END.h"
#include "EVENT_ADULTHOOD_THUNDER.h"
#include "EVENT_ADULTHOOD_TURTLE.h"
#include "EVENT_ADULTHOOD_NOTURTLE.h"
#include "EVENT_ADULTHOOD_UPSTAR.h"
#include "EVENT_ADULTHOOD_GIRL.h"
#include "EVENT_ADULTHOOD_RHYTHM.h"
#include "EVENT_ADULTHOOD_WEDDING.h"
#include "AdultPlayer.h"


EventMachine* EventMachine::eventManager = nullptr;

EventMachine::EventMachine()
	:
#pragma region INIT
	_currentState(nullptr),
	_preState(TITLE),
	_newState(_preState),
	_states{}
#pragma endregion
{
	// 스테이트 생성
	// 첫 시작 STATE TITLE
	_states[TITLE] = new EVENT_TITLE(this);
	_states[INTRO] = new EVENT_INTRO(this);
	_states[INFANCY_FARMING] = new EVENT_INFANCY_FARMING(this);
	_states[INFANCY_SHAKING] = new EVENT_INFANCY_SHAKING(this);
	_states[INFANCY_END] = new EVENT_INFANCY_END(this);
	
	_states[YOUTH_INTRO] = new EVENT_YOUTH_INTRO(this);
	_states[YOUTH_ROW] = new EVENT_YOUTH_ROW(this);
	_states[YOUTH_FARMING] = new EVENT_YOUTH_FARMING(this);
	_states[YOUTH_SOLO] = new EVENT_YOUTH_SOLO(this);

	_states[ADULTHOOD_INTRO] = new EVENT_ADULTHOOD_INTRO(this);
	_states[ADULTHOOD_SEAGULL] = new EVENT_ADULTHOOD_SEAGULL(this);
	_states[ADULTHOOD_COMPETITOR] = new EVENT_ADULTHOOD_COMPETITOR(this);
	_states[ADULTHOOD_SWIRL] = new EVENT_ADULTHOOD_SWIRL(this);
	_states[ADULTHOOD_THUNDER] = new EVENT_ADULTHOOD_THUNDER(this);
	_states[ADULTHOOD_TURTLE] = new EVENT_ADULTHOOD_TURTLE(this);
	_states[ADULTHOOD_NOTURTLE] = new EVENT_ADULTHOOD_NOTURTLE(this);
	_states[ADULTHOOD_UPSTAR] = new EVENT_ADULTHOOD_UPSTAR(this);
	_states[ADULTHOOD_GIRL] = new EVENT_ADULTHOOD_GIRL(this);
	_states[ADULTHOOD_RHYTHM] = new EVENT_ADULTHOOD_RHYTHM(this);
	_states[ADULTHOOD_WEDDING] = new EVENT_ADULTHOOD_WEDDING(this);

	_states[OLDAGE_INTRO] = new  EVENT_OLDAGE_INTRO(this);
	_states[OLDAGE_FLIASHING] = new EVENT_OLDAGE_FLIASHING(this);
	_states[OLDAGE_END] = new EVENT_OLDAGE_END(this);

	_currentState = _states[_preState];
	_currentState->Start();
}

EventMachine::~EventMachine()
{}

EventMachine* EventMachine::GetInstance()
{
	if (eventManager == nullptr)
		eventManager = new EventMachine();
	
	return eventManager;
}

void EventMachine::Release()
{
	// 스테이트 소멸
	for (auto statePair : _states)
	{
		delete statePair.second;
	}
}

void EventMachine::ClearData()
{
}

void EventMachine::Update()
{
	_newState = _currentState->Update();

	while (_newState != _preState)
	{
		_currentState->End();					// 이전 스테이트 꺼지기 전 동작
		_currentState = _states[_newState];		// 바꿔끼워주고
		_currentState->Start();					// 새로운 스테이트 시작 동작
		_preState = _newState;
		_newState = _currentState->Update();	// 새로운 스테이트 업데이트
	}
}

void EventMachine::SetEventNextFatherText(int next)
{
	_mainTalkPanel->GetScript<InfancyText>()->_selectText = next;
}

void EventMachine::SetNextFatherText(int next)
{
	_mainTalkPanel->GetScript<InfancyText>()->_nowText = next;
}

// panel을 true할때 이걸로 어떤 패널의 이미지를 set해줄지 정해줘야함
void EventMachine::SetPanelImage(int idnum)
{
	switch (idnum)
	{
	case (int)IDNUM::FATHER:
	{
		_mainTalkPanel->GetComponent<Panel>()->SetImages(L"father_base.png");
	}
	break;
	case (int)IDNUM::MOM:
	{
		_mainTalkPanel->GetComponent<Panel>()->SetImages(L"mom_base.png");
	}
	break;
	case (int)IDNUM::DODO:
	{
		_mainTalkPanel->GetComponent<Panel>()->SetImages(L"speech bubble_Dodo.png");
	}
	break;
	case (int)IDNUM::SEAGULL:
	{
		_mainTalkPanel->GetComponent<Panel>()->SetImages(L"seagull_base.png");
	}
	break;
	case(int)IDNUM::SEAGULL_YES_NO:
	{
		_mainTalkPanel->GetComponent<Panel>()->SetImages(L"seagull_yes_no.png");
	}
	break;
	case (int)IDNUM::ETC:
	{
		_mainTalkPanel->GetComponent<Panel>()->SetImages(L"mom_base.png");
	}
	break;
	case (int)IDNUM::Lucy:
	{
		_mainTalkPanel->GetComponent<Panel>()->SetAlpha(0.5f);
		_mainTalkPanel->GetComponent<Panel>()->SetIsAlpha(true);

		_mainTalkPanel->GetComponent<Panel>()->SetImages(L"speech bubble_Lucy.png");
	}
	break;
	case (int)IDNUM::Turtle:
	{
		_mainTalkPanel->GetComponent<Panel>()->SetImages(L"turtle_r.png");
	}
	break;
	case (int)IDNUM::DODO2:
	{
		_mainTalkPanel->GetComponent<Panel>()->SetAlpha(0.5f);
		_mainTalkPanel->GetComponent<Panel>()->SetIsAlpha(true);

		_mainTalkPanel->GetComponent<Panel>()->SetImages(L"dodo_2.png");
	}
	break;
	case (int)IDNUM::Star:
	{
		_mainTalkPanel->GetComponent<Panel>()->SetAlpha(0.f);
		_mainTalkPanel->GetComponent<Panel>()->SetIsAlpha(true);
	}
	break;
	default:
		break;
	}
}

void EventMachine::SetEventYouthNextText(int next)
{
	_mainTalkPanel->GetScript<YouthText>()->_selectText = next;
}

void EventMachine::SetNextYouthtText(int next)
{
	_mainTalkPanel->GetScript<YouthText>()->_nowText = next;
}

void EventMachine::SetEventAdultNextText(int next)
{
	_mainTalkPanel->GetScript<AdulthoodText>()->_selectText = next;
}

void EventMachine::SetNextAdultText(int next)
{
	_mainTalkPanel->GetScript<AdulthoodText>()->_nowText = next;
}

void EventMachine::SetEventOldAgeNextText(int next)
{
	_mainTalkPanel->GetScript<OldAgeText>()->_selectText = next;
}

void EventMachine:: SetNextOldAgeText(int next)
{
	_mainTalkPanel->GetScript<OldAgeText>()->_nowText = next;
}

bool EventMachine::GetSeaGullEventCheck()
{
	return _boat->GetComponent<AdultPlayer>()->GetSeaGullCheck();
}

void EventMachine::SetWave(shared_ptr<GameObject> wave)
{
	_wave = wave;
}

void EventMachine::SetCancelFarming(bool check)
{
	if(_boat !=nullptr)
	_boat->GetScript<Player>()->SetFarmingCancel(check);
}

void EventMachine::ResetMachine()
{
	_mainTalkPanel.reset();
	_FlisingImage.reset();
	_camera.reset();
	_boat.reset();
	_fadeInOutPanel.reset();
	_dodo.reset();
	_grandMa.reset();
	_wave.reset();
	_yesButton.reset();
	_noButton.reset();
	_spacePanel.reset();
	_arrowPanel.reset();
	_skyBox.reset();
	_girl.reset();
	_girlBoat.reset();

	// 스테이트 소멸
	for (auto statePair : _states)
	{
		delete statePair.second;
	}

	// 스테이트 재 생성
	_states[TITLE] = new EVENT_TITLE(this);
	_states[INTRO] = new EVENT_INTRO(this);
	_states[INFANCY_FARMING] = new EVENT_INFANCY_FARMING(this);
	_states[INFANCY_SHAKING] = new EVENT_INFANCY_SHAKING(this);
	_states[INFANCY_END] = new EVENT_INFANCY_END(this);

	_states[YOUTH_INTRO] = new EVENT_YOUTH_INTRO(this);
	_states[YOUTH_ROW] = new EVENT_YOUTH_ROW(this);
	_states[YOUTH_FARMING] = new EVENT_YOUTH_FARMING(this);
	_states[YOUTH_SOLO] = new EVENT_YOUTH_SOLO(this);

	_states[ADULTHOOD_INTRO] = new EVENT_ADULTHOOD_INTRO(this);
	_states[ADULTHOOD_SEAGULL] = new EVENT_ADULTHOOD_SEAGULL(this);
	_states[ADULTHOOD_COMPETITOR] = new EVENT_ADULTHOOD_COMPETITOR(this);
	_states[ADULTHOOD_SWIRL] = new EVENT_ADULTHOOD_SWIRL(this);
	_states[ADULTHOOD_THUNDER] = new EVENT_ADULTHOOD_THUNDER(this);
	_states[ADULTHOOD_TURTLE] = new EVENT_ADULTHOOD_TURTLE(this);
	_states[ADULTHOOD_NOTURTLE] = new EVENT_ADULTHOOD_NOTURTLE(this);
	_states[ADULTHOOD_UPSTAR] = new EVENT_ADULTHOOD_UPSTAR(this);
	_states[ADULTHOOD_GIRL] = new EVENT_ADULTHOOD_GIRL(this);
	_states[ADULTHOOD_RHYTHM] = new EVENT_ADULTHOOD_RHYTHM(this);
	_states[ADULTHOOD_WEDDING] = new EVENT_ADULTHOOD_WEDDING(this);

	_states[OLDAGE_INTRO] = new  EVENT_OLDAGE_INTRO(this);
	_states[OLDAGE_FLIASHING] = new EVENT_OLDAGE_FLIASHING(this);
	_states[OLDAGE_END] = new EVENT_OLDAGE_END(this);

	_currentState = _states[TITLE];
	_currentState->Start();

	isIntro = false;
	isFarming = false;				
	isInfacnyShaking = false;
	isInfacnyEnd = false;
	isYouthIntro = false;
	isYouthRow = false;
	isYouthIndependent = false;
	isAdulthoodIntro = false;
	isAdultSeaGull = false;
	isAdultCompetitor = false;
	isAdultSwirl = false;
	isAdultThunder = false;
	isAdultUpstar = false;
	isAdultGirl = false;
	isOldAgeIntro = false;
	isOldAgeFlishing = false;
	isOldAgeEnd = false;
	isMove = true;
	isSellDream = false;
	isSelectedYes = false;
	isSelectedNo = false;
	isShakingDeath = false;
	isSellDreamTalk = false;
	isBadEnding = false;
	isHappyEnding = false;
}
