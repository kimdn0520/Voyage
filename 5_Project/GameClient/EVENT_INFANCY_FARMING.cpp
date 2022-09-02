#include "pch.h"
#include "EVENT_INFANCY_FARMING.h"
#include "EventMachine.h"
#include "HeartPrefab.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "InfancyText.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Player.h"
#include "GameManager.h"
#include "Panel.h"
#include "InputManager.h"
#include "Item.h"
#include "Transform.h"

#include <random>

EVENT_INFANCY_FARMING::EVENT_INFANCY_FARMING(EventMachine* ref)
	: ref(ref)
{}

EVENT_INFANCY_FARMING::~EVENT_INFANCY_FARMING()
{
}

void EVENT_INFANCY_FARMING::Start()
{
	_curSpawnTime = 0.f;
	_curheartCnt = 0.f;
	_nowText = 0;
	_isHeart1 = false;
	_isHeart2 = false;
	_isHeart3 = false;

	_isAddHeart1 = false;
	_isAddHeart2 = false;
	_isAddHeart3 = false;
	_isAddHeart4 = false;
	_isAddHeart5 = false;

	ref->_spacePanel->SetActive(true);
	ref->SetEventNextFatherText(1);
	ref->SetNextFatherText(_nowText);

	_heartIdx = 0;
}

int EVENT_INFANCY_FARMING::Update()
{
	if (SkipNextEvent())
	{
		return EventMachine::INFANCY_SHAKING;
	}

	SkipNextScene();

	// �׾��ٰ� ��Ƴ���..!
	if (ref->isShakingDeath)
	{
		_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		// FadeOut�� �����ٸ�..
		if (_alpha <= 0.01f)
		{
			ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(0.f);

			_alpha = 0.f;

			//ref->_fadeInOutPanel->GetComponent<Panel>()->SetIsAlpha(_isFade);

			GameManager::GetInstance()->items[ItemType::Heart] = 5;

			ref->isFarming = true;

			ref->isShakingDeath = false;
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}
	else
	{
		ScriptCheck();
		Text();
		ref->SetNextFatherText(_nowText);
		ref->_wave->activeSelf = ref->_boat->GetScript<Player>()->MoveCheck();

		_curSpawnTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		_curTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		if (_curSpawnTime > 2.f && _curheartCnt < 30)
		{
			// TODO : �����ϰ� ��Ʈ������ ���� ������ ����������
			std::random_device rd;
			std::mt19937 gen(rd());
			uniform_int_distribution<int> dis(-90, 90);

			shared_ptr<HeartPrefab> heartPrefab = make_shared<HeartPrefab>(Vector3(dis(gen), 0.5f, 100.f));
			heartPrefab->gameObject->GetScript<Item>()->SetPlus(_itemFloat);
			SceneManager::GetInstance()->SetInstantiateGameObject(heartPrefab->gameObject);

			_itemFloat *= -1;
			_curheartCnt++;
			_curSpawnTime = 0.f;
		}

		// ��ȹ�� �°� ��ġ��
		if (_curTime >= 65.f)
		{
			_isLastText = true;
			Text();
			return EventMachine::INFANCY_SHAKING;
		}
	}

	return EventMachine::INFANCY_FARMING;
}

void EVENT_INFANCY_FARMING::End()
{
	ref->isFarming = false;
	ref->SetCancelFarming(true);
	ref->SetEventNextFatherText(2);
	_nowText = 0;
	_curSpawnTime = 0.f;
	_curTime = 0.f;
	_curheartCnt = 0.f;
	_talkTime = 0.f;

	_isHeart1 = false;
	_isHeart2 = false;
	_isHeart3 = false;

	_isAddHeart1 = false;
	_isAddHeart2 = false;
	_isAddHeart3 = false;
	_isAddHeart4 = false;
	_isAddHeart5 = false;

	for (auto heart : _heartPrefab)
		SceneManager::GetInstance()->SetRemoveGameObject(heart->gameObject);

	_heartIdx = 0;

}

void EVENT_INFANCY_FARMING::Text()
{
	_outTextTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	// ������ ���� �ð��Ŀ� �ؽ�Ʈ�� ����
	if (_outTextTime >= 5.0f && !_isFirstText)
	{
		//ref->_isMainTalkOnOff = true;
		ref->_mainTalkPanel->GetScript<InfancyText>()->_isPanelCheck = true;
		ref->_mainTalkPanel->SetActive(true);
		_isFirstText = true;
	}
	// ù��° �ؽ�Ʈ�� ���Դٴ°� �����Ǹ�
	else if (_isFirstText)
	{
		ref->_spacePanel->SetActive(false);
		// ���ͼ� ��簡 ������ϰ� �б�Ǽ� ��簡 �߰� �����ð��̻�Ǹ� ���������
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;
		if (_talkTime > 2.0f)
		{
			ref->_mainTalkPanel->SetActive(false);
			_talkTime = 0.f;
		}

		// ��Ʈ�� ���� �б�Ǿ����
		// ��Ʈ�� 3����
		if (GameManager::GetInstance()->items[ItemType::Heart] == 3 && !_isHeart1)
		{
			_talkTime = 0.f;
			ref->_mainTalkPanel->SetActive(true);
			_isHeart1 = true;
			_isAddHeart1 = true;

			_nowText++;
		}
		else if (GameManager::GetInstance()->items[ItemType::Heart] == 18 && !_isAddHeart1)
		{
			_heartPrefab.push_back(make_shared<HeartPrefab>(Vector3(0.f, 2.f, -2.f)));
			_heartPrefab[_heartIdx]->gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
			_heartPrefab[_heartIdx]->gameObject->GetScript<Item>()->SetIsFarmingItem(true);
			SceneManager::GetInstance()->SetInstantiateGameObject(_heartPrefab[_heartIdx]->gameObject);
			_heartPrefab[_heartIdx]->gameObject->GetComponent<Transform>()->SetParent(ref->_boat->GetTransform());
			_heartIdx++;
			_isAddHeart1 = true;
		}
		else if (GameManager::GetInstance()->items[ItemType::Heart] == 6 && !_isAddHeart2)
		{

			_heartPrefab.push_back(make_shared<HeartPrefab>(Vector3(1.f, 2.f, -2.f)));
			_heartPrefab[_heartIdx]->gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
			_heartPrefab[_heartIdx]->gameObject->GetScript<Item>()->SetIsFarmingItem(true);
			SceneManager::GetInstance()->SetInstantiateGameObject(_heartPrefab[_heartIdx]->gameObject);
			_heartPrefab[_heartIdx]->gameObject->GetComponent<Transform>()->SetParent(ref->_boat->GetTransform());
			_isAddHeart2 = true;
			_heartIdx++;
		}
		else if (GameManager::GetInstance()->items[ItemType::Heart] == 7 && !_isHeart2)
		{
			_nowText++;
			_talkTime = 0.f;
			ref->_mainTalkPanel->SetActive(true);
			_isHeart2 = true;
		}
		else if (GameManager::GetInstance()->items[ItemType::Heart] == 9 && !_isAddHeart3)
		{

			_heartPrefab.push_back(make_shared<HeartPrefab>(Vector3(-1.f, 2.f, -2.f)));
			_heartPrefab[_heartIdx]->gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
			_heartPrefab[_heartIdx]->gameObject->GetScript<Item>()->SetIsFarmingItem(true);
			SceneManager::GetInstance()->SetInstantiateGameObject(_heartPrefab[_heartIdx]->gameObject);
			_heartPrefab[_heartIdx]->gameObject->GetComponent<Transform>()->SetParent(ref->_boat->GetTransform());
			_isAddHeart3 = true;
			_heartIdx++;

		}
		else if (GameManager::GetInstance()->items[ItemType::Heart] == 12 && !_isAddHeart4)
		{

			_heartPrefab.push_back(make_shared<HeartPrefab>(Vector3(-0.5f, 2.f, -2.f)));
			_heartPrefab[_heartIdx]->gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
			_heartPrefab[_heartIdx]->gameObject->GetScript<Item>()->SetIsFarmingItem(true);
			SceneManager::GetInstance()->SetInstantiateGameObject(_heartPrefab[_heartIdx]->gameObject);
			_heartPrefab[_heartIdx]->gameObject->GetComponent<Transform>()->SetParent(ref->_boat->GetTransform());
			_isAddHeart4 = true;
			_heartIdx++;

		}
		else if (GameManager::GetInstance()->items[ItemType::Heart] == 13 && !_isHeart3)
		{
			_nowText++;
			_talkTime = 0.f;
			ref->_mainTalkPanel->SetActive(true);
			_isHeart3 = true;
		}
		else if (GameManager::GetInstance()->items[ItemType::Heart] == 15 && !_isAddHeart5)
		{
			_heartPrefab.push_back(make_shared<HeartPrefab>(Vector3(0.5f, 2.f, -2.f)));
			_heartPrefab[_heartIdx]->gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
			_heartPrefab[_heartIdx]->gameObject->GetScript<Item>()->SetIsFarmingItem(true);
			SceneManager::GetInstance()->SetInstantiateGameObject(_heartPrefab[_heartIdx]->gameObject);
			_heartPrefab[_heartIdx]->gameObject->GetComponent<Transform>()->SetParent(ref->_boat->GetTransform());
			_isAddHeart5 = true;
			_heartIdx++;

		}
		
		if (_isLastText)
		{
			ref->_mainTalkPanel->SetActive(false);
		}

	}
}

void EVENT_INFANCY_FARMING::ScriptCheck()
{
	int talker = ref->_mainTalkPanel->GetScript<InfancyText>()->ReturnTalker2(_nowText);
	ref->SetPanelImage(talker);
}

bool EVENT_INFANCY_FARMING::SkipNextEvent()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F3))
	{
		return true;
	}
	return false;

}

void EVENT_INFANCY_FARMING::SkipNextScene()
{
	if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::F4))
	{
		// ������ �ε��� ���̸��� Set���ְ�
		SceneManager::GetInstance()->SetLoadSceneName("YouthScene");

		// �ε������� �Ѿ��.
		SceneManager::GetInstance()->LoadScene("LoadingScene");
	}
}
