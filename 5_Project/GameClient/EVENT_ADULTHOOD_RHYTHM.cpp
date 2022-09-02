#include "pch.h"
#include "EVENT_ADULTHOOD_RHYTHM.h"
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
#include "MusicNotePrefab.h"
#include "DataManager.h"
#include "AdultBoyPrefab.h"
#include "AdultGirlPrefab.h"
#include "OldAgeText.h"
#include "GameManager.h"
#include "AdultPlayer.h"
#include "BigBoatPrefab.h"
#include "Resources.h"
#include <random>
#include "Item.h"
#include "AdulthoodText.h"
#include "MusicNotePrefab.h"


EVENT_ADULTHOOD_RHYTHM::EVENT_ADULTHOOD_RHYTHM(EventMachine* ref)
	: ref(ref)
{}

EVENT_ADULTHOOD_RHYTHM::~EVENT_ADULTHOOD_RHYTHM()
{}

void EVENT_ADULTHOOD_RHYTHM::Start()
{
	_curSpawnTime = 0.f;
	_curheartCnt = 0.f;
	_nowText = 0;

	_isNote1 = false;
	_isNote2 = false;
	_isNote3 = false;

	_isAddNote1 = false;
	_isAddNote2 = false;
	_isAddNote3 = false;
	_isAddNote4 = false;
	_isAddNote5 = false;

	ref->isFarming = true;

	// 이거 확인 필요
	ref->SetEventAdultNextText(7);
	ref->SetNextAdultText(_nowText);

	ref->isMove = true;
	ref->_girlBoat->GetTransform()->SetParent(ref->_boat->GetTransform());

	ref->_girlBoat->GetTransform()->SetLocalPosition(Vector3(5.f, 0.f, 0.f));
	ref->_girlBoat->GetTransform()->SetLocalScale(Vector3(0.9f, 0.9f, 0.9f));

	_NoteIdx = 0;

}

int EVENT_ADULTHOOD_RHYTHM::Update()
{
	if (SkipNextEvent())
	{
		return EventMachine::INFANCY_SHAKING;
	}

	StreamNote();
	SkipNextScene();

	Text();

	// 그다음 씬
	if (ref->isAdultRhythm)
		return EventMachine::ADULTHOOD_WEDDING;
	return EventMachine::ADULTHOOD_RHYTHM;
}

void EVENT_ADULTHOOD_RHYTHM::End()
{
	ref->isFarming = true;
	//ref->SetCancelFarming(true);
	// 이거 맞는 텍스트 
	//ref->SetNextAdultText(9);
	_nowText = 0;

	_curSpawnTime = 0.f;
	_curTime = 0.f;
	_curheartCnt = 0.f;
	_talkTime = 0.f;

	bool _isNote1 = false;
	bool _isNote2 = false;
	bool _isNote3 = false;
	bool _isNote4 = false;
	bool _isNote5 = false;

	bool _isAddNote1 = false;
	bool _isAddNote2 = false;
	bool _isAddNote3 = false;
	bool _isAddNote4 = false;
	bool _isAddNote5 = false;

	for (auto note : _musicNotePrefab)
		SceneManager::GetInstance()->SetRemoveGameObject(note->gameObject);
	_NoteIdx = 0;
}

bool EVENT_ADULTHOOD_RHYTHM::SkipNextEvent()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F3))
	{
		return true;
	}
	return false;
}

void EVENT_ADULTHOOD_RHYTHM::SkipNextScene()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F4))
	{
		// 다음에 로드할 씬이름을 Set해주고
		SceneManager::GetInstance()->SetLoadSceneName("YouthScene");

		// 로딩씬으로 넘어간다.
		SceneManager::GetInstance()->LoadScene("LoadingScene");
	}
}

void EVENT_ADULTHOOD_RHYTHM::StreamNote()
{
	_curSpawnTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	_curTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	if (_curSpawnTime > 2.f && _curNoteCnt < 30)
	{
		// TODO : 랜덤하게 백합아이템 생성 서서히 내려오게함
		std::random_device rd;
		std::mt19937 gen(rd());
		uniform_int_distribution<int> dis(-90, 90);

		shared_ptr<MusicNotePrefab> musicNotePrefab = make_shared<MusicNotePrefab>(Vector3(dis(gen), 0.8f, 100.f));
		musicNotePrefab->gameObject->GetScript<Item>()->SetPlus(_itemFloat);
		SceneManager::GetInstance()->SetInstantiateGameObject(musicNotePrefab->gameObject);


		_curNoteCnt++;
		_curSpawnTime = 0.f;
		_itemFloat *= -1;
	}
	if (_curTime > 65.f)
	{
		ref->isAdultRhythm = true;
		return;
	}
}

void EVENT_ADULTHOOD_RHYTHM::Talk()
{
	/*if (_isTalk)
	{
		ref->_mainTalkPanel->SetActive(true);
		ref->_mainTalkPanel->GetScript<OldAgeText>()->_isPanelCheck = true;
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		if (_talkTime > 2.0f)
		{
			ref->SetEventAdultNextText(_nowText);
			ScriptCheck();
			_talkTime = 0.f;
			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text9Size() == _nowText)
		{
			_isTalk = false;
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);
		}
	}
	else
	{
		ref->isOldAgeFlishing = true;
	}*/
}

void EVENT_ADULTHOOD_RHYTHM::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker9(_nowText);
	ref->SetPanelImage(talker);
}

void EVENT_ADULTHOOD_RHYTHM::Text()
{
	_outTextTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;


	// 들어오면 일정 시간후에 텍스트를 켜줌
	if (_outTextTime > 2.0f)
	{
		//ref->_isMainTalkOnOff = true;
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
		ref->_mainTalkPanel->SetActive(false);
		_outTextTime = 0.f;

		if (_isLastText)
		{
			ref->isAdultRhythm = true;
			return;
		}
	}

	if (GameManager::GetInstance()->items[ItemType::Note] == 2 && !_isNote1)
	{
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
		ref->_mainTalkPanel->SetActive(true);
		ref->SetNextAdultText(_nowText);
		_nowText++;
		_outTextTime = 0.f;
		_isNote1 = true;
	}
	// 첫번째 텍스트가 나왔다는게 인정되면
	else if (_isNote1 && GameManager::GetInstance()->items[ItemType::Note] == 4)
	{
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
		ref->_mainTalkPanel->SetActive(true);
		ref->SetNextAdultText(_nowText);
		_outTextTime = 0.f;
		_nowText++;
		_isNote1 = false;
		_isNote2 = true;
	}

	// 노트에 따라 분기되어야함
	// 노트가 3개면
	else if (GameManager::GetInstance()->items[ItemType::Note] == 6 && _isNote2)
	{
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
		ref->_mainTalkPanel->SetActive(true);
		ref->SetNextAdultText(_nowText);
		_outTextTime = 0.f;
		_nowText++;
		_isNote2 = false;
		_isNote3 = true;
	}
	else if (GameManager::GetInstance()->items[ItemType::Note] == 8 && _isNote3)
	{
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
		ref->_mainTalkPanel->SetActive(true);
		ref->SetNextAdultText(_nowText);
		_outTextTime = 0.f;
		_nowText++;
		_isNote3 = false;
		_isNote4 = true;
	}
	else if (GameManager::GetInstance()->items[ItemType::Note] == 9 && _isNote4)
	{
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
		ref->_mainTalkPanel->SetActive(true);
		ref->SetNextAdultText(_nowText);
		_outTextTime = 0.f;
		_nowText++;
		_isNote4 = false;
		_isNote5 = true;
	}

	else if (GameManager::GetInstance()->items[ItemType::Note] == 10 && _isNote5)
	{
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
		ref->_mainTalkPanel->SetActive(true);
		ref->SetNextAdultText(_nowText);
		_outTextTime = 0.f;
		_nowText++;
		_isNote5 = false;
		_isLastText = true;

	}


}
