#include "pch.h"
#include <random>
#include "EVENT_ADULTHOOD_TURTLE.h"
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
#include "SwirlPrefab.h"
#include "SphereCollider.h"
#include "Island2Prefab.h"
#include "TurtlePrefab.h"
#include "Resources.h"
#include "MeshRenderer.h"

EVENT_ADULTHOOD_TURTLE::EVENT_ADULTHOOD_TURTLE(EventMachine* ref)
	: ref(ref)
{

}

EVENT_ADULTHOOD_TURTLE::~EVENT_ADULTHOOD_TURTLE()
{}

void EVENT_ADULTHOOD_TURTLE::Start()
{
	Resources::GetInstance()->SetCubeMapTexture(ref->_skyBox->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"galaxy.dds");

	// 섬을 생성한다. 
	_island2Prefab = make_shared<Island2Prefab>(Vector3(50.f, 7.f, 150.f));
	SceneManager::GetInstance()->SetInstantiateGameObject(_island2Prefab->gameObject);

	hash<string> hasher;

	// 거북이를 생성한다. 
	_turtlePrefab = make_shared<TurtlePrefab>(Vector3(1.f, 0.3f, -3.f));
	_turtlePrefab->gameObject->GetTransform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
	_turtlePrefab->gameObject->GetTransform()->SetLocalRotation(Vector3(0.f, -1.f, 5.f));
	_turtlePrefab->gameObject->PlayAnim(_turtlePrefab->gameObject, hasher("TurtleArmatureAction"), true);
	SceneManager::GetInstance()->SetInstantiateGameObject(_turtlePrefab->gameObject);

	// 거북이의 부모로 섬을 넣는다. 
	_turtlePrefab->gameObject->GetComponent<Transform>()->SetParent(_island2Prefab->gameObject->GetTransform());

	// 이번에 나올 텍스트
	_nowText = 0;
	ref->SetEventAdultNextText(15);
	ref->SetNextAdultText(_nowText);

	_blinkTime = 3.f;
	_alpha = 1.f;
	_isTalk = true;
	_isFade = true;
	_isLastFade = false;

	ref->isMove = true;

	_isCameraMove = true;

	_isSellDreamTalk_1 = true;
}

int EVENT_ADULTHOOD_TURTLE::Update()
{
	//_curTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	if (ref->_boat != nullptr)
		ref->_wave->activeSelf = ref->_boat->GetScript<AdultPlayer>()->MoveCheck();

	if (_isFade)
	{
		_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha <= 0.01f)
		{
			_isFade = false;

			_alpha = 0.f;
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}

	if (_isLastFade)
	{
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha >= 1.f)
		{
			_isLastFade = false;

			_alpha = 1.f;

			ref->_fadeInOutPanel->GetComponent<Panel>()->SetImages(L"black.png");

			// 다음에 로드할 씬이름을 Set해주고
			SceneManager::GetInstance()->SetLoadSceneName("BadEndingScene");

			// 로딩씬으로 넘어간다.
			SceneManager::GetInstance()->LoadScene("BadEndingScene");
		}

		ref->_fadeInOutPanel->GetComponent<Panel>()->SetAlpha(_alpha);
	}

	// 거북이와 충돌시 이벤트 시작
	if (ref->isSellDreamTalk)
	{
		if (_isCameraMove)
		{
			CameraMove();
		}
		else
		{
			SellDreamScript_1();

			SellDreamScript_2();

			if(_isCameraStarMove)
				CameraStarMove();

			SellDreamScript_3();
		}
	}

	if (ref->isBadEnding)
	{
		ref->_fadeInOutPanel->GetComponent<Panel>()->SetImages(L"Bad ending scene.png");
		ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
		ref->_mainTalkPanel->SetActive(false);
		_blinkTime = 5.f;

		_isLastFade = true;

		ref->isBadEnding = false;
	}

	return EventMachine::ADULTHOOD_TURTLE;
}

void EVENT_ADULTHOOD_TURTLE::End()
{
	
}

/// <summary>
/// 거북이와 트리거작용시 나오게 하자
/// </summary>
void EVENT_ADULTHOOD_TURTLE::SellDreamScript_1()
{
	if (_isSellDreamTalk_1)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		// 2초마다 말하는중
		if (_talkTime > 2.0f)
		{
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
			ref->_mainTalkPanel->SetActive(true);

			SellDreamScriptCheck_1();

			ref->SetNextAdultText(_nowText);
			_talkTime = 0.f;
			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text17Size() == _nowText)
		{
			_isSellDreamTalk_1 = false;

			_isSellDreamTalk_2 = true;

			_isCameraStarMove = true;

			_nowText = 0;

			ref->SetNextAdultText(_nowText);

			ref->SetEventAdultNextText(16);
		}
	}
}

void EVENT_ADULTHOOD_TURTLE::SellDreamScript_2()
{
	if (_isSellDreamTalk_2)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		// 2초마다 말하는중
		if (_talkTime > 2.0f)
		{
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
			ref->_mainTalkPanel->SetActive(true);

			SellDreamScriptCheck_2();

			ref->SetNextAdultText(_nowText);
			_talkTime = 0.f;
			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text18Size() == _nowText)
		{
			_isSellDreamTalk_2 = false;

			_isSellDreamTalk_3 = true;

			_nowText = 0;

			ref->SetNextAdultText(_nowText);

			ref->SetEventAdultNextText(17);
		}
	}
}

void EVENT_ADULTHOOD_TURTLE::SellDreamScript_3()
{
	if (_isSellDreamTalk_3)
	{
		_talkTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		// 2초마다 말하는중
		if (_talkTime > 2.0f)
		{
			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = true;
			ref->_mainTalkPanel->SetActive(true);

			SellDreamScriptCheck_3();

			ref->SetNextAdultText(_nowText);
			_talkTime = 0.f;
			_nowText++;
		}

		if (ref->_mainTalkPanel->GetScript<AdulthoodText>()->Text19Size() == _nowText)
		{
			_isSellDreamTalk_3 = false;

			ref->_mainTalkPanel->GetScript<AdulthoodText>()->_isPanelCheck = false;
			ref->_mainTalkPanel->SetActive(false);
			
			// 배드 엔딩 
			// 거북이랑 섬 지우기
			SceneManager::GetInstance()->SetRemoveGameObject(_island2Prefab->gameObject);
			SceneManager::GetInstance()->SetRemoveGameObject(_turtlePrefab->gameObject);

			// 마지막 섬 생성
			shared_ptr<Island2Prefab> island2Prefab = make_shared<Island2Prefab>(Vector3(0.f, 7.f, 150.f));
			island2Prefab->gameObject->SetTag(Tag::BadEnding);
			SceneManager::GetInstance()->SetInstantiateGameObject(island2Prefab->gameObject);

			// 카메라 위치 셋팅
			ref->_camera->GetTransform()->SetLocalPosition(Vector3(0.f, 150.f, 0.f));
			ref->_camera->GetTransform()->LookAt(Vector3(0, 0, 0));

			// 플레이어 다시 움직임 
			ref->isMove = true;
		}
	}
}

void EVENT_ADULTHOOD_TURTLE::SellDreamScriptCheck_1()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker17(_nowText);
	ref->SetPanelImage(talker);
}

void EVENT_ADULTHOOD_TURTLE::SellDreamScriptCheck_2()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker18(_nowText);
	ref->SetPanelImage(talker);
}

void EVENT_ADULTHOOD_TURTLE::SellDreamScriptCheck_3()
{
	int talker = ref->_mainTalkPanel->GetScript<AdulthoodText>()->ReturnTalker19(_nowText);
	ref->SetPanelImage(talker);
}

void EVENT_ADULTHOOD_TURTLE::CameraMove()
{
	if (_turtleSumCameraTrans <= _turtleCameraTrans)
	{
		ref->_camera->GetTransform()->MoveLook(12.f * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed);
		ref->_camera->GetTransform()->MoveUp(-5.f * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed);
		ref->_camera->GetTransform()->MoveRight(3.f * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed);

		_turtleSumCameraTrans += 10.f * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;
	}
	else
	{
		float rotateVal = 0.5f * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;
		_totalCameraTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

		if (_turtleSumCamerRotateX <= _turtleCamerRotateX)
		{
			if (_totalCameraTime > 0.01f)
			{
				// 카메라 올려줌 
				_turtleSumCamerRotateX += rotateVal;
				ref->_camera->GetTransform()->RotateXAxis(-rotateVal);
				_totalCameraTime = 0.f;
			}
		}
		else
		{
			_turtleSumCamerRotateX = 0.f;
			_totalCameraTime = 0.f;
			_isCameraMove = false;
		}
	}
}

void EVENT_ADULTHOOD_TURTLE::CameraStarMove()
{
	float rotateVal = 0.3f * TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;
	
	_totalCameraTime += TimeManager::GetInstance()->GetDeltaTime() * GameManager::GetInstance()->scriptSpeed;

	if (_turtleSumCamerRotateX <= _turtleCamerRotateX)
	{
		if (_totalCameraTime > 0.01f)
		{
			// 카메라 올려줌 
			_turtleSumCamerRotateX += rotateVal;
			ref->_camera->GetTransform()->RotateXAxis(-rotateVal);
			_totalCameraTime = 0.f;
		}
	}
	else
	{
		if (_turtleSumCamerRotateX_2 >= _turtleCamerRotateX_2)
		{
			if (_totalCameraTime > 0.01f)
			{
				// 카메라 내려줌 
				_turtleSumCamerRotateX_2 -= rotateVal;

				ref->_camera->GetTransform()->RotateXAxis(rotateVal);
				
				_totalCameraTime = 0.f;
			}
		}
	}
}
