#include "pch.h"
#include "LoadingScene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Resources.h"
#include "MeshRenderer.h"
#include "Text.h"
#include "LoadingText.h"
#include "Panel.h"
#include "LoadSceneScript.h"
#include "GameProcess.h"
#include "NewbieEngineFunc.h"
#include "Audio.h"

void LoadingScene::Init()
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
	background->AddComponent<LoadSceneScript>();
	AddGameObject(background);

	shared_ptr<GameObject> loadingImg = make_shared<GameObject>();
	loadingImg->SetName("loadingImg");
	loadingImg->AddComponent<Transform>();
	loadingImg->AddComponent<Panel>();
	loadingImg->GetComponent<Panel>()->SetOffset(0.f, GameProcess::_windowInfo.width, 0.f, GameProcess::_windowInfo.height);
	loadingImg->GetComponent<Panel>()->SetImages(L"Loading.png");
	AddGameObject(loadingImg);

	shared_ptr<GameObject> boatImg = make_shared<GameObject>();
	boatImg->SetName("BoatImg");
	boatImg->AddComponent<Transform>();
	boatImg->AddComponent<Panel>();
	boatImg->GetComponent<Panel>()->SetOffset(0.f, GameProcess::_windowInfo.width, 0.f, GameProcess::_windowInfo.height);
	boatImg->GetComponent<Panel>()->SetImages(L"boatImg.png");
	AddGameObject(boatImg);

	ResizeWindow(GameProcess::resizeInfo.width, GameProcess::resizeInfo.height);
}