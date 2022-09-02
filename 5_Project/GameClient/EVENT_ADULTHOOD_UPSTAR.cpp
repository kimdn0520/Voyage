#include "pch.h"
#include "EVENT_ADULTHOOD_UPSTAR.h"
#include "EventMachine.h"
#include "StarPrefab.h"
#include "BigStarPrefab.h"
#include "GameObject.h"
#include "Item.h"
#include "SceneManager.h"
#include "Transform.h"
#include "TimeManager.h"
#include "Panel.h"
#include "GameManager.h"
#include "AdulthoodText.h"
#include "AdultPlayer.h"

EVENT_ADULTHOOD_UPSTAR::EVENT_ADULTHOOD_UPSTAR(EventMachine* ref)
	: ref(ref)
{

}

EVENT_ADULTHOOD_UPSTAR::~EVENT_ADULTHOOD_UPSTAR()
{

}

void EVENT_ADULTHOOD_UPSTAR::Start()
{
	// 첫 시작에 움직임을 막는다. 
	ref->isMove = false;
	Vector3 pos = ref->_boat->GetComponent<Transform>()->GetLocalPosition();
	int temp = -1;
	vector<Vector3> posvector;
	posvector.push_back(Vector3(pos.x - 10, pos.y, pos.z + 80.f));
	posvector.push_back(Vector3(pos.x + 6, pos.y, pos.z + 85.f));
	posvector.push_back(Vector3(pos.x - 25, pos.y, pos.z + 100.f));
	posvector.push_back(Vector3(pos.x + 20, pos.y, pos.z + 80.f));
	posvector.push_back(Vector3(pos.x - 30, pos.y, pos.z + 90.f));

	_itemTime.resize(5);
	_itemY.resize(5);
	for (int i = 0; i < 5; i++)
	{
		_starsObj.push_back(make_shared<StarPrefab>(posvector[i]));
		temp *= -1;
		_starsObj[i]->gameObject->GetScript<Item>()->SetIsFarmingItem(true);
		_starsObj[i]->gameObject->GetTransform()->SetLocalRotation(Vector3(90.f, 0.f, 0.f));
		SceneManager::GetInstance()->SetInstantiateGameObject(_starsObj[i]->gameObject);
		_isStarsCheck.push_back(false);
		_itemTime[i] = 0.f;
		_itemY[i] = 0.f;
	}
	_bigStar = make_shared<BigStarPrefab>(Vector3(Vector3(pos.x, pos.y + 5, pos.z + 80.0f)));
	_bigStar->gameObject->GetScript<Item>()->SetIsFarmingItem(true);
	_bigStar->gameObject->GetTransform()->SetLocalRotation(Vector3(90.f, 0.f, 0.f));
	SceneManager::GetInstance()->SetInstantiateGameObject(_bigStar->gameObject);
	_isBigStarCheck = false;
	_cameraPos = ref->_camera->GetTransform()->GetLocalPosition();

	_blinkTime = 3.f;
	_alpha = 0.f;
	_isFade = true;
	_isCameraDown = false;
	_nowText = 0;
	ref->SetEventAdultNextText(6);
	ref->SetNextAdultText(_nowText);
	ref->isAdultUpstar = false;
}

int EVENT_ADULTHOOD_UPSTAR::Update()
{
	// 페이드인 혹시 필요할까봐 남겨둠
	//if (_isFade)
	//{
	//	_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

	//	if (_alpha <= 0.01f)
	//	{
	//		_isFade = false;

	//		_alpha = 0.f;
	//	}

	//	ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	//}
	Talk();
	ScriptCheck();
	UpStar();

	if (_isLastFadeCheck)
	{
		ref->_fadeInOutPanel->GetComponent<Panel>()->SetImages(L"white.png");
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha >= 1.0f)
		{
			_isLastFadeCheck = false;
			_alpha = 1.0f;
			ref->isAdultUpstar = true;
			ref->_camera->GetTransform()->LookAt(Vector3(0, 0, 0));
			ref->_boat->GetScript<AdultPlayer>()->SetStartRotate();
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}
	if (ref->isAdultUpstar)
		return EventMachine::ADULTHOOD_GIRL;

	return EventMachine::ADULTHOOD_UPSTAR;
}

void EVENT_ADULTHOOD_UPSTAR::End()
{
	for (int i = 0; i < 5; i++)
		SceneManager::GetInstance()->SetRemoveGameObject(_starsObj[i]->gameObject);
	SceneManager::GetInstance()->SetRemoveGameObject(_bigStar->gameObject);


	_isCameraDown = false;
}

void EVENT_ADULTHOOD_UPSTAR::UpStar()
{
	float rotatescale = 0.01f;
	_itemY[0] = 10.f;
	_itemY[1] = -13.f;
	_itemY[2] = 0.f;
	_itemY[3] = -5.f;
	_itemY[4] = 3.f;

	for (int i = 0; i < 5; i++)
	{

		if (_starsObj[i]->gameObject->GetTransform()->GetLocalPosition().z < 150.0f && _starsObj[i]->gameObject->GetTransform()->GetLocalPosition().y < 150.0f + _itemY[i])
		{
			_starsObj[i]->gameObject->GetTransform()->MoveUp(TimeManager::GetInstance()->GetDeltaTime() * 27.0f);
			_isStarsCheck[i] = false;
		}
		else
			_isStarsCheck[i] = true;
	}

	if (_bigStar->gameObject->GetTransform()->GetLocalPosition().z < 150.0f && _bigStar->gameObject->GetTransform()->GetLocalPosition().y < 150.0f + 5.f)
	{
		_bigStar->gameObject->GetTransform()->MoveUp(TimeManager::GetInstance()->GetDeltaTime() * 27.0f);
		_isBigStarCheck = false;
	}
	else
		_isBigStarCheck = true;
	IntroCameraAnimation();

}

bool EVENT_ADULTHOOD_UPSTAR::IntroCameraAnimation()
{
	float rotatescale = 0.01f;
	_totalCameraTime += TimeManager::GetInstance()->GetDeltaTime() * 1.f;

	if (_introSumCamerRotateX <= _introCamerRotateX)
	{
		if (_totalCameraTime > 0.04f)
		{
			// 카메라 올려줌 
			_introSumCamerRotateX += rotatescale;
			ref->_camera->GetTransform()->RotateXAxis(-rotatescale);
			_totalCameraTime = 0.f;
		}
	}
	else
		_isTalkCheck = true;
	return false;
}

void EVENT_ADULTHOOD_UPSTAR::Talk()
{
	_talkTime += TimeManager::GetInstance()->GetDeltaTime();
	if (_isTalkCheck)
	{
		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text7Size() == _nowText)
		{
			_isLastFadeCheck = true;
			return;
		}
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
		ref->_mainTalkPanel->SetActive(true);
		ref->SetNextAdultText(_nowText);

		// 말이 흘러나와야함
		if (_talkTime > 3.0f)
		{
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);

			_talkTime = 0.f;
			_nowText++;
		}
	}

}

void EVENT_ADULTHOOD_UPSTAR::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker7(_nowText);
	ref->SetPanelImage(talker);
}