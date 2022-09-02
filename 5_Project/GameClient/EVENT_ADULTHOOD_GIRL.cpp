#include "pch.h"
#include "EVENT_ADULTHOOD_GIRL.h"
#include "EventMachine.h"
#include "AdultGirlPrefab.h"
#include "SceneManager.h"
#include "Transform.h"
#include "BoatPrefab.h"
#include "GameObject.h"
#include "Panel.h"
#include "TimeManager.h"
#include "AdulthoodText.h"

EVENT_ADULTHOOD_GIRL::EVENT_ADULTHOOD_GIRL(EventMachine* ref)
	: ref(ref)
{

}

EVENT_ADULTHOOD_GIRL::~EVENT_ADULTHOOD_GIRL()
{

}

void EVENT_ADULTHOOD_GIRL::Start()
{
	// 배를 생성한다.
	shared_ptr<BoatPrefab> boatPrefab = make_shared<BoatPrefab>(Vector3(20.f, 3.f, -80.f));

	// 소녀를 생성한다. 
	shared_ptr<AdultGirlPrefab> girlPrefab = make_shared<AdultGirlPrefab>(Vector3(-0.7f, 0.f, 1.5f));
	girlPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	SceneManager::GetInstance()->SetInstantiateGameObject(girlPrefab->gameObject);

	// 배를 설정해준다.
	SceneManager::GetInstance()->SetInstantiateGameObject(boatPrefab->gameObject);

	girlPrefab->gameObject->GetComponent<Transform>()->SetParent(boatPrefab->gameObject->GetTransform());

	// 이벤트 머신에 넣어주자~
	ref->_girl = girlPrefab->gameObject;
	ref->_girlBoat = boatPrefab->gameObject;

	ref->isMove = false;
	//_alpha = 1.f;

	_isGirlCome = false;
	_isTalk = false;
	_comingTime = 0.f;
	_nowText = 0;
	_isFade = true;
	ref->SetEventAdultNextText(21);
	ref->SetNextAdultText(_nowText);
}

int EVENT_ADULTHOOD_GIRL::Update()
{
	// 처음 할때 페이드 인이 필요
	if (_isFade)
	{
		ref->_fadeInOutPanel->GetComponent<Panel>()->SetImages(L"white.png");
		_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha <= 0.01f)
		{
			_isFade = false;

			_alpha = 0.f;

			_isGirlCome = true;
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}
	ComeGirl();
	Talk();

	if (ref->isAdultGirl)
		return EventMachine::ADULTHOOD_RHYTHM;
	return EventMachine::ADULTHOOD_GIRL;
}

void EVENT_ADULTHOOD_GIRL::End()
{
	_isGirlCome = false;
	_isTalk = false;
	_comingTime = 0.f;
}

void EVENT_ADULTHOOD_GIRL::ComeGirl()
{
	if (_isGirlCome)
	{
		_comingTime += TimeManager::GetInstance()->GetDeltaTime();

		if (_comingTime > 0.01f)
		{
			_moveRange += (TimeManager::GetInstance()->GetDeltaTime() * 5.f);

			if ((-80.f + _moveRange) < -25)
			{
				ref->_girlBoat->GetTransform()->SetLocalPosition(Vector3(20.f, 3.f, -80.f + _moveRange));
				_comingTime = 0.f;
			}
			else
			{
				ref->_girlBoat->GetTransform()->SetLocalPosition(Vector3(20.f, 3.f, -25.f));
				_isGirlCome = false;
				_isTalk = true;
			}
		}
	}
}

void EVENT_ADULTHOOD_GIRL::Talk()
{
	if (_isTalk)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime();
		
		if (_talkTime > 2.0f)
		{
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
			ref->_mainTalkPanel->SetActive(true);
			ScriptCheck();

			ref->SetNextAdultText(_nowText);
			_talkTime = 0.f;
			_nowText++;
		}
		
		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text8Size() == _nowText)
		{
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

			_isTalk = false;
			ref->isAdultGirl = true;
			_talkTime = 0.f;
			return;
		}
	}
}

void EVENT_ADULTHOOD_GIRL::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker8(_nowText);
	ref->SetPanelImage(talker);
}
