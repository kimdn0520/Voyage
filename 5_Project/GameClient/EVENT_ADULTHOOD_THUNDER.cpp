#include "pch.h"
#include "EVENT_ADULTHOOD_THUNDER.h"
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
#include "AdultPlayer.h"
#include "MoveObject.h"
#include "IslandPrefab.h"
#include "CameraScript.h"
#include "BoatPrefab.h"
#include "DataManager.h"
#include "AdultBoyPrefab.h"
#include "AdultGirlPrefab.h"
#include "BirdPrefab.h"
#include "AdulthoodText.h"
#include "GameManager.h"
#include "DodoYouthPrefab.h"
#include "BigBoatPrefab.h"
#include "BirdDiePrefab.h"
#include "LightningPrefab.h"
#include "Animator.h"
#include "Audio.h"

EVENT_ADULTHOOD_THUNDER::EVENT_ADULTHOOD_THUNDER(EventMachine* ref)
	: ref(ref)
{

}

EVENT_ADULTHOOD_THUNDER::~EVENT_ADULTHOOD_THUNDER()
{

}

void EVENT_ADULTHOOD_THUNDER::Start()
{
	// 섬을 생성한다. 
	_islandPrefab = make_shared<IslandPrefab>(Vector3(-55.f, 0.f, 150.f));
	//islandPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(4.f, 4.f, 4.f));
	SceneManager::GetInstance()->SetInstantiateGameObject(_islandPrefab->gameObject);

	// 갈매기를 생성한다. 
	_birdPrefab = make_shared<BirdPrefab>(Vector3(1.f, -2.f, 2.f));
	_birdPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(2.f, 2.f, 2.f));
	_birdPrefab->gameObject->GetTransform()->SetLocalRotation(Vector3(30.f, 165.f, 0.f));
	SceneManager::GetInstance()->SetInstantiateGameObject(_birdPrefab->gameObject);

	// 갈매기를 생성한다. 
	_birdDiePrefab = make_shared<BirdDiePrefab>(Vector3(1.f, -2.f, 2.f));
	_birdDiePrefab->gameObject->GetTransform()->SetLocalScale(Vector3(2.f, 2.f, 2.f));
	_birdDiePrefab->gameObject->GetTransform()->SetLocalRotation(Vector3(30.f, 165.f, 0.f));
	_birdDiePrefab->gameObject->SetActive(false);
	SceneManager::GetInstance()->SetInstantiateGameObject(_birdDiePrefab->gameObject);

	// 섬에 자식으로 갈매기를 넣는다. 
	_birdPrefab->gameObject->GetComponent<Transform>()->SetParent(_islandPrefab->gameObject->GetTransform());
	_birdDiePrefab->gameObject->GetComponent<Transform>()->SetParent(_islandPrefab->gameObject->GetTransform());

	_nowText = 0;
	ref->SetEventAdultNextText(5);
	ref->SetNextAdultText(_nowText);
	ref->isAdultSeaGull = false;
	_isTalk = false;
	_isThunderCheck = false;
	_isFade = false;
	_isTalkEventCheck = false;
	_isFadeIn = false;
	_isLastFade = false;
	_isBirdDieAnim = true;
	_blinkTime = 2.f;
	ref->_fadeInOutPanel->GetComponent<Panel>()->SetImages(L"white.png");
}

int EVENT_ADULTHOOD_THUNDER::Update()
{
	ScriptCheck();
	if (ref->_boat != nullptr)
		ref->_wave->activeSelf = ref->_boat->GetScript<AdultPlayer>()->MoveCheck();

	// 갈매기와 충돌시 이벤트 시작
	if (ref->isAdultSeaGull)
	{
		Talk();
	}

	Thunder();

	/*if (_isLastFade)
	{
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha >= 1.0f)
		{
			_isLastFade = false;

			_alpha = 1.0f;

			ref->isAdultThunder = true;
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);

	}*/

	if (ref->isAdultThunder)
	{
		// 꿈을 팔면 거북이 엔딩
		if (ref->isSellDream)
			return EventMachine::ADULTHOOD_TURTLE;
		// 꿈을 안팔면 그 다음으로 넘어감
		else
			return EventMachine::ADULTHOOD_NOTURTLE;

	}
	return EventMachine::ADULTHOOD_THUNDER;
}

void EVENT_ADULTHOOD_THUNDER::End()
{
	_isThunderCheck = false;
	_isFade = false;
	_isTalkEventCheck = false;

}

void EVENT_ADULTHOOD_THUNDER::Talk()
{
	_talkTime += TimeManager::GetInstance()->GetDeltaTime();

	if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text6Size() == _nowText)
	{
		_isLastFade = true;
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha >= 1.0f)
		{

			_alpha = 1.0f;

			// 씬에 있는 새랑 섬을 없애준다. 
			SceneManager::GetInstance()->SetRemoveGameObject(_birdDiePrefab->gameObject);
			SceneManager::GetInstance()->SetRemoveGameObject(_islandPrefab->gameObject);

			ref->isAdultThunder = true;
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
		return;
	}
	ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
	ref->_mainTalkPanel->SetActive(true);
	ref->SetNextAdultText(_nowText);

	if (_talkTime > 2.0f)
	{
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
		ref->_mainTalkPanel->SetActive(false);
	}
	// 2초마다 말하는중
	if (_talkTime > 2.0f && !_isTalkEventCheck)
	{
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
		ref->_mainTalkPanel->SetActive(false);

		//ref->SetNextAdultText(_nowText);
		_isTalkEventCheck = true;
		_isFade = true;
		_talkTime = 0.f;
		_nowText++;
	}
	else if (_talkTime > 4.0f && _isFadeIn)
	{
		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text16Size() == _nowText)
		{
			_isFade = true;

			_isThunderCheck = true;
			return;
		}

		if (_isBirdDieAnim)
		{
			hash<string> hasher;
			_birdDiePrefab->gameObject->PlayAnim(_birdDiePrefab->gameObject, hasher("bird_islandAction.001"), false);
			_isBirdDieAnim = false;
		}

		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
		ref->_mainTalkPanel->SetActive(false);

		ref->SetNextAdultText(_nowText);
		_talkTime = 0.f;
		_nowText++;
	}
}

void EVENT_ADULTHOOD_THUNDER::Thunder()
{
	if (_isThunderCheck)
	{
		_birdPrefab->gameObject->SetActive(false);
		SceneManager::GetInstance()->SetRemoveGameObject(_birdPrefab->gameObject);
		_birdDiePrefab->gameObject->SetActive(true);
		hash<string> hasher;
		_birdDiePrefab->gameObject->PlayAnim(_birdDiePrefab->gameObject, hasher("none"), false);
		_isThunderCheck = false;
	}


	if (_isFade)
	{
		ref->_fadeInOutPanel->GetComponent<Panel>()->SetImages(L"white.png");
		_lightTime += TimeManager::GetInstance()->GetDeltaTime();
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha >= 0.5f)
		{
			_isFade = false;
			_isThunderCheck = true;
			_alpha = 0.5f;

		}
		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}
	else if (!_isFade && !_isLastFade)
	{
		ref->_fadeInOutPanel->GetComponent<Panel>()->SetImages(L"black.png");
		_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha <= 1.0f)
		{
			_isFade = false;
			_isFadeIn = true;
			_alpha = 0.0f;

		}
		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}
}


void EVENT_ADULTHOOD_THUNDER::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker6(_nowText);
	ref->SetPanelImage(talker);
}

