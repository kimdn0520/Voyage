#include "pch.h"
#include "TitleScene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Resources.h"
#include "MeshRenderer.h"
#include "Text.h"
#include "TextScript.h"
#include "DataManager.h"
#include "Button.h"
#include "Panel.h"
#include "PressAnyKey.h"
#include "BlinkImage.h"
#include "GameProcess.h"
#include "SoundManager.h"
#include "Audio.h"

void TitleScene::Init()
{
	//---------------------------------------------------------------------------------------------------------
	// 메인 카메라
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->SetName("MainCamera");
	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>();
	//camera->AddComponent<Audio>();
	camera->AddComponent<CameraScript>();
	AddGameObject(camera);

	shared_ptr<GameObject> background = make_shared<GameObject>();
	background->SetName("BackGround");
	background->AddComponent<Transform>();
	background->AddComponent<Panel>();
	background->GetComponent<Panel>()->SetOffset(0.f, GameProcess::_windowInfo.width, 0.f, GameProcess::_windowInfo.height);
	background->GetComponent<Panel>()->SetImages(L"sea.png");
	background->GetComponent<Panel>()->SetSortLayer(0);
	AddGameObject(background);

	shared_ptr<GameObject> titleLogo = make_shared<GameObject>();
	titleLogo->SetName("TitleLogo");
	titleLogo->AddComponent<Transform>();
	titleLogo->AddComponent<Panel>();
	titleLogo->GetComponent<Panel>()->SetOffset(0.f, GameProcess::_windowInfo.width, 0.f, GameProcess::_windowInfo.height);
	titleLogo->GetComponent<Panel>()->SetImages(L"logo.png");
	titleLogo->GetComponent<Panel>()->SetSortLayer(1);
	AddGameObject(titleLogo);

	shared_ptr<GameObject> pressAnyKey = make_shared<GameObject>();
	pressAnyKey->SetName("PressAnyKey");
	pressAnyKey->AddComponent<Transform>();
	pressAnyKey->AddComponent<Panel>();
	pressAnyKey->GetComponent<Panel>()->SetOffset(670.f, 1340.f, 700.f, 850.f);
	pressAnyKey->GetComponent<Panel>()->SetImages(L"PressAnyKey.png");
	pressAnyKey->GetComponent<Panel>()->SetIsAlpha(true);
	pressAnyKey->GetComponent<Panel>()->SetAlpha(0.f);
	pressAnyKey->GetComponent<Panel>()->SetSortLayer(2);
	pressAnyKey->AddComponent<BlinkImage>();
	AddGameObject(pressAnyKey);

	shared_ptr<GameObject> fadeOut = make_shared<GameObject>();
	fadeOut->SetName("FadeOut");
	fadeOut->AddComponent<Transform>();
	fadeOut->AddComponent<Panel>();
	fadeOut->GetComponent<Panel>()->SetOffset(0.f, GameProcess::_windowInfo.width, 0.f, GameProcess::_windowInfo.height);
	fadeOut->GetComponent<Panel>()->SetImages(L"black.png");
	fadeOut->GetComponent<Panel>()->SetIsAlpha(true);
	fadeOut->GetComponent<Panel>()->SetAlpha(0.f);
	fadeOut->GetComponent<Panel>()->SetSortLayer(3);
	fadeOut->AddComponent<PressAnyKey>();
	AddGameObject(fadeOut);

	shared_ptr<GameObject> soundManager = make_shared<GameObject>();
	soundManager->SetName("SoundManager");
	soundManager->AddComponent<Transform>();
	soundManager->AddComponent<Audio>();
	soundManager->AddComponent<SoundManager>();
	AddGameObject(soundManager);
}