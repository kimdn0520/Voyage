#include "pch.h"
#include "EVENT_ADULTHOOD_WEDDING.h"
#include "EventMachine.h"
#include "Panel.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "RosePrefab.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "Item.h"
#include "AdulthoodText.h"

#include <random>

EVENT_ADULTHOOD_WEDDING::EVENT_ADULTHOOD_WEDDING(EventMachine* ref)
	: ref(ref)
{

}

EVENT_ADULTHOOD_WEDDING::~EVENT_ADULTHOOD_WEDDING()
{

}

void EVENT_ADULTHOOD_WEDDING::Start()
{
	_nowText = 0;
	_isFade = true;
	ref->SetEventAdultNextText(8);
	ref->SetNextAdultText(_nowText);
}

int EVENT_ADULTHOOD_WEDDING::Update()
{
	// 처음 할때 페이드 인이 필요
	if (_isFade)
	{
		ref->_fadeInOutPanel->GetComponent<Panel>()->SetImages(L"black.png");
		_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha <= 0.01f)
		{
			_isFade = false;

			_alpha = 0.f;

			_isTalk = true;
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}
	Farming();
	Talk();
	
	// 끝날때 페이드 아웃
	if (_isLastFade)
	{
		ref->_fadeInOutPanel->GetComponent<Panel>()->SetImages(L"black.png");
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha >= 1.0f)
		{
			_isLastFade = false;

			_alpha = 1.0f;
			// 다음에 로드할 씬이름을 Set해주고
			SceneManager::GetInstance()->SetLoadSceneName("OldScene");

			// 로딩씬으로 넘어간다.
			SceneManager::GetInstance()->LoadScene("LoadingScene");

		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}

	return EventMachine::ADULTHOOD_WEDDING;
}

void EVENT_ADULTHOOD_WEDDING::End()
{

}

void EVENT_ADULTHOOD_WEDDING::Farming()
{
	_curSpawnTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	_curTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	if (_curSpawnTime > 2.f && _curRoseCnt < 30)
	{
		// TODO : 랜덤하게 백합아이템 생성 서서히 내려오게함
		std::random_device rd;
		std::mt19937 gen(rd());
		uniform_int_distribution<int> dis(-90, 90);

		shared_ptr<RosePrefab> lilyFlowerPrefab = make_shared<RosePrefab>(Vector3(dis(gen), 0.f, 100.f));
		lilyFlowerPrefab->gameObject->AddComponent<Item>();
		lilyFlowerPrefab->gameObject->GetScript<Item>()->SetType(ItemType::LilyFlower);

		SceneManager::GetInstance()->SetInstantiateGameObject(lilyFlowerPrefab->gameObject);

		_curRoseCnt++;
		_curSpawnTime = 0.f;
	}
	if (_curTime > 65.f)
	{
		ref->isAdultWedding = true;
		return;
	}
}

void EVENT_ADULTHOOD_WEDDING::Talk()
{
	if (_isTalk)
	{
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
		ref->_mainTalkPanel->SetActive(true);

		_talkTime += TimeManager::GetInstance()->GetDeltaTime();
		if (_talkTime > 2.0f)
		{
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

		}
		if (_talkTime > 2.0f && _nowText < 3)
		{

			ref->SetNextAdultText(_nowText);
			_talkTime = 0.f;
			_nowText++;
		}
		if ((_nowText == 3) && !_isMusic)
		{
			ref->SetNextAdultText(_nowText);
			_nowText++;
			_isJump = false;
			_isMusic = true;
		}
		if (_curRoseCnt == 2 && _nowText == 4)
		{
			if (!_isCheck1)
			{
				ref->SetNextAdultText(_nowText);
				_nowText++;
				_isCheck1 = true;
				_isJump = true;
			}
		}
		else if (_curRoseCnt != 4 || _nowText != 6)
		{
			_isJump = false;
		}
		else if (_curRoseCnt == 4 && _nowText == 6)
		{
			if (!_isCheck2)
			{
				ref->SetNextAdultText(_nowText);
				_nowText++;
				_isCheck2 = true;
				_isJump = true;
			}
		}
		else if (_curRoseCnt != 6 || _nowText != 10)
		{
			_isJump = false;

		}
		else if (_curRoseCnt == 6 && _nowText == 10)
		{
			if (!_isCheck3)
			{
				ref->SetNextAdultText(_nowText);
				_nowText++;
				_isCheck3 = true;
				_isJump = true;

			}
		}
		else if (_curRoseCnt == 8 && _nowText == 12)
		{
			if (!_isCheck4)
			{
				ref->SetNextAdultText(_nowText);
				_nowText++;
				_isCheck4 = true;
			}
		}
		else
		{
			if (_isJump)
			{
				ref->SetNextAdultText(_nowText);
				_nowText++;
			}
		}

		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text8Size() == _nowText)
		{
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

			_isTalk = false;
			_talkTime = 0.f;
			ref->isAdultWedding = true;
			_isLastFade = true;
			return;
		}

	}
}
