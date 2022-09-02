#include "pch.h"
#include "HappyEndingScene.h"
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
#include "NewbieEngineFunc.h"
#include "HappyEndingSceneScript.h"

void HappyEndingScene::Init()
{
	//---------------------------------------------------------------------------------------------------------
	// 메인 카메라
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->SetName("MainCamera");
	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraScript>();
	AddGameObject(camera);

	shared_ptr<GameObject> background = make_shared<GameObject>();
	background->SetName("BackGround");
	background->AddComponent<Transform>();
	background->AddComponent<Panel>();
	background->GetComponent<Panel>()->SetOffset(0.f, GameProcess::_windowInfo.width, 0.f, GameProcess::_windowInfo.height);
	background->GetComponent<Panel>()->SetImages(L"Happy ending scene 1.png");
	background->GetComponent<Panel>()->SetSortLayer(0);
	background->AddComponent<HappyEndingSceneScript>();
	AddGameObject(background);

	/*shared_ptr<GameObject> soundManager = make_shared<GameObject>();
	soundManager->SetName("SoundManager");
	soundManager->AddComponent<Transform>();
	soundManager->AddComponent<Audio>();
	soundManager->AddComponent<SoundManager>();
	soundManager->GetScript<SoundManager>()->TitleSound();
	AddGameObject(soundManager);*/

	ResizeWindow(GameProcess::resizeInfo.width, GameProcess::resizeInfo.height);
}