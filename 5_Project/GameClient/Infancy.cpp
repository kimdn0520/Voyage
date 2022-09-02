#include "pch.h"
#include "Infancy.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Resources.h"
#include "MeshRenderer.h"
#include "Light.h"
#include "MeshData.h"
#include "Animator.h"
#include "Text.h"
#include "TextScript.h"
#include "Player.h"
#include "DataManager.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Button.h"
#include "Panel.h"
#include "Canvas.h"
#include "UIManager.h"
#include "GameProcess.h"
#include "Audio.h"
#include "HeartPrefab.h"
#include "SmallBoatPrefab.h"
#include "StarPrefab.h"
#include "BigStarPrefab.h"
#include "ClockPrefab.h"
#include "MoneyPrefab.h"
#include "AdultGirlPrefab.h"
#include "LilyFlowerPrefab.h"
#include "BottlePrefab.h"
#include "RosePrefab.h"
#include "MusicNotePrefab.h"
#include "GlacierPrefab.h"
#include "AudioListener.h"
#include "BoatPrefab.h"
#include "AdultBoyPrefab.h"
#include "BigBoatPrefab.h"
#include "EventMachine.h"
#include "SoundManager.h"
#include "AudioListener.h"
#include "IslandPrefab.h"
#include "Island2Prefab.h"
#include "TurtlePrefab.h"
#include "InfancyText.h"
#include "BirdPrefab.h"
#include "BirdDiePrefab.h"
#include "BirdDieScript.h"
#include "NewbieEngineFunc.h"
#include "BabyPrefab.h"
#include "Audio.h"

void Infancy::Init()
{
	if (!DataManager::GetInstance()->isDataLoad)
		DataManager::GetInstance()->DataLoad();


	EventMachine::GetInstance()->isIntro = true;

	//---------------------------------------------------------------------------------------------------------
	// Main Camera
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->SetName("MainCamera");
	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>();
	camera->AddComponent<Audio>();
	camera->AddComponent<CameraScript>();
	camera->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 150.f, 0.f));
	camera->GetComponent<Transform>()->LookAt(Vector3(0.f, 0.f, 0.f));
	AddGameObject(camera);
	//EventManager::GetInstance()->SetCamera(camera);
	EventMachine::GetInstance()->SetCamera(camera);

	//---------------------------------------------------------------------------------------------------------
	// SkyBox
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> skyBox = make_shared<GameObject>();
	skyBox->SetName("SkyBox");
	skyBox->AddComponent<Transform>();
	skyBox->AddComponent<MeshRenderer>();
	skyBox->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::SphereMesh);
	skyBox->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = DataManager::GetInstance()->GetSkyBoxId();
	skyBox->GetComponent<MeshRenderer>()->GetMeshInfo()->type = OBJECT_TYPE::SKY_BOX;
	Resources::GetInstance()->SetCubeMapTexture(skyBox->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"daylight.dds");
	AddGameObject(skyBox);


	//---------------------------------------------------------------------------------------------------------
	// Water 
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> water = make_shared<GameObject>();
	water->SetName("water");
	water->AddComponent<Transform>();
	water->AddComponent<MeshRenderer>();
	water->AddComponent<BoxCollider>();
	water->GetComponent<BoxCollider>()->SetColliderInfo(Vector3(0.f, -15.f, 50.f), Vector3(200.f, 1.f, 200.f));
	water->GetComponent<BoxCollider>()->SetIsTrigger(true);
	water->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
	//water->GetComponent<Transform>()->SetLocalRotation(Vector3(90.f, 0.f, 0.f));
	water->GetComponent<Transform>()->SetLocalScale(Vector3(165.f, 0.f, 82.f));
	water->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::RectMesh);
	water->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = DataManager::GetInstance()->GetOceanID();
	water->GetComponent<MeshRenderer>()->GetMeshInfo()->type = OBJECT_TYPE::WATER;
	Resources::GetInstance()->SetNormalMapTexture(water->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"water_normal.dds");

	AddGameObject(water);

	//---------------------------------------------------------------------------------------------------------
	// Water Camera!
	//---------------------------------------------------------------------------------------------------------
	Vector3 center = water->GetComponent<Transform>()->GetLocalPosition();			// water의 중심

	Vector3 targets[6] =
	{
		Vector3(center.x + 1.0f, center.y, center.z),
		Vector3(center.x, center.y + 1.0f, center.z),
		Vector3(center.x, center.y + 1.0f, center.z),
		Vector3(center.x, center.y - 1.0f, center.z),
		Vector3(center.x, center.y, center.z + 1.0f),
		Vector3(center.x, center.y, center.z - 1.0f)
	};

	vector<shared_ptr<GameObject>> subCam(6);

	for (int i = 0; i < 6; i++)
	{
		subCam[i] = make_shared<GameObject>();
		subCam[i]->SetName("waterCamera" + i);
		subCam[i]->AddComponent<Transform>();
		subCam[i]->AddComponent<Camera>();
		subCam[i]->GetComponent<Transform>()->SetLocalPosition(center);
		subCam[i]->GetComponent<Transform>()->LookAt(targets[i]);
		subCam[i]->GetComponent<Camera>()->SetObjectType(OBJECT_TYPE::WATER);
		AddGameObject(subCam[i]);
	}

	subCam[2]->GetComponent<Transform>()->RotateYAxis(3.141592f);

	shared_ptr<GameObject> wave = make_shared<GameObject>();
	wave->SetName("wave");
	wave->AddComponent<Transform>();
	wave->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 3.f, -8.f));
	wave->GetComponent<Transform>()->SetLocalScale(Vector3(1.5f, 2.f, 2.f));
	wave->AddComponent<MeshRenderer>();
	wave->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::WaveMesh);
	wave->GetComponent<MeshRenderer>()->GetMeshInfo()->type = OBJECT_TYPE::WaveEffect;
	wave->SetActive(false);
	AddGameObject(wave);

	// 머신으로 넘겨줘야함
	// 씬마다 필요~
	EventMachine::GetInstance()->SetWave(wave);

	//---------------------------------------------------------------------------------------------------------
	// Directional Light
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> dirLight = make_shared<GameObject>();
	dirLight->SetName("dirLight");
	dirLight->AddComponent<Transform>();
	dirLight->AddComponent<Light>();
	dirLight->GetComponent<Transform>()->SetLocalPosition(Vector3(20000.f, 20000.f, 20000.f));
	dirLight->GetComponent<Light>()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
	dirLight->GetComponent<Light>()->SetLightDirection(Vector3(0.57735f, -0.57735f, 0.57735f));
	dirLight->GetComponent<Light>()->SetDiffuse(Vector4(0.6f, 0.6f, 0.6f, 1.0f));	// 기본0.5 
	dirLight->GetComponent<Light>()->SetAmbient(Vector4(0.6f, 0.6f, 0.6f, 1.0f));	// 기본0.2
	dirLight->GetComponent<Light>()->SetSpecular(Vector4(0.2f, 0.2f, 0.2f, 1.0f));  // 기본0.5
	dirLight->GetComponent<Light>()->SendLightInfo();
	AddGameObject(dirLight);

	Vector3 smallBoatpos = water->GetComponent<Transform>()->GetWorldPosition();
	smallBoatpos.y = -200;
	//subCam[1]->GetComponent<Transform>()->RotateYAxis(3.141592f);
	subCam[1]->GetComponent<Transform>()->SetLocalPosition(smallBoatpos);
	
	//--------------------------------------------------------------------------------------------------------
	// ASE Model - example2
	//---------------------------------------------------------------------------------------------------------
	/*shared_ptr<BabyPrefab> babyPrefab = make_shared<BabyPrefab>(Vector3(20.f, 4.f, 18.f));
	babyPrefab->gameObject->GetTransform()->SetLocalScale(Vector3(10.f, 10.f, 10.f));
	AddGameObject(babyPrefab->gameObject);
	hash<string> hasher;
	babyPrefab->gameObject->PlayAnim(babyPrefab->gameObject, hasher("Sleep"));*/


#pragma region UI
	// 디버그 정보 텍스트
	shared_ptr<GameObject> testText = make_shared<GameObject>();
	testText->SetName("testText");
	testText->AddComponent<Text>();
	testText->AddComponent<TextScript>();
	AddGameObject(testText);

	shared_ptr<GameObject> heartGaugeBack = make_shared<GameObject>();
	heartGaugeBack->AddComponent<Transform>();
	heartGaugeBack->AddComponent<Panel>();
	heartGaugeBack->GetComponent<Panel>()->SetOffset(
		0 + 70.f,
		GameProcess::_windowInfo.width / 2 - 620.f,
		GameProcess::_windowInfo.height / 2 + 440.f,
		GameProcess::_windowInfo.height / 2 + 490.f);
	heartGaugeBack->GetComponent<Panel>()->SetImages(L"HP_1.png");
	heartGaugeBack->GetComponent<Panel>()->SetSortLayer(1);
	AddGameObject(heartGaugeBack);

	shared_ptr<GameObject> heartGauge = make_shared<GameObject>();
	heartGauge->AddComponent<Transform>();
	heartGauge->AddComponent<Panel>();
	heartGauge->GetComponent<Panel>()->SetOffset(
		0 + 120.f,
		0 + 120.f,
		GameProcess::_windowInfo.height / 2 + 450.f,
		GameProcess::_windowInfo.height / 2 + 480.f);
	heartGauge->GetComponent<Panel>()->SetImages(L"HP_2.png");
	heartGauge->GetComponent<Panel>()->SetSortLayer(0);
	AddGameObject(heartGauge);

	//shared_ptr<GameObject> heartImage = make_shared<GameObject>();
	//heartImage->AddComponent<Transform>();
	//heartImage->AddComponent<Panel>();
	//heartImage->GetComponent<Panel>()->SetOffset(
	//	0.f,
	//	GameProcess::_windowInfo.width / 2 - 850.f,
	//	GameProcess::_windowInfo.height / 2 + 400.f,
	//	GameProcess::_windowInfo.height / 2 + 530.f);
	//heartImage->GetComponent<Panel>()->SetImages(L"HP_1.png");
	//heartImage->GetComponent<Panel>()->SetSortLayer(2);
	//AddGameObject(heartImage);

	// TalkPanel
	// father
	shared_ptr mainTalkPanel = make_shared<GameObject>();
	mainTalkPanel->SetName("SceneMainTalkPanel");
	mainTalkPanel->AddComponent<Transform>();
	mainTalkPanel->AddComponent<Panel>();
	mainTalkPanel->GetComponent<Panel>()->SetOffset(
		GameProcess::_windowInfo.width / 2 - 600.f,
		GameProcess::_windowInfo.width / 2 + 600.f,
		GameProcess::_windowInfo.height-600.f,
		GameProcess::_windowInfo.height + 10.f);
	mainTalkPanel->GetComponent<Panel>()->SetImages(L"father_base.png");
	mainTalkPanel->GetComponent<Panel>()->SetSortLayer(0);
	mainTalkPanel->AddComponent<Text>();
	mainTalkPanel->GetComponent<Text>()->SetLocation(650, 920, GameProcess::_windowInfo.width+100, GameProcess::_windowInfo.height-50);
	mainTalkPanel->AddComponent<InfancyText>();
	mainTalkPanel->SetActive(false);
	AddGameObject(mainTalkPanel);
	EventMachine::GetInstance()->SetMainTalkPanel(mainTalkPanel);

	//spage
	shared_ptr spacePanel = make_shared<GameObject>();
	spacePanel->SetName("spacePanel");
	spacePanel->AddComponent<Transform>();
	spacePanel->AddComponent<Panel>();
	spacePanel->GetComponent<Panel>()->SetOffset(
		GameProcess::_windowInfo.width / 2 - 100,
		GameProcess::_windowInfo.width / 2 + 100,
		GameProcess::_windowInfo.height / 2 - 150,
		GameProcess::_windowInfo.height / 2 + 50);
	spacePanel->GetComponent<Panel>()->SetImages(L"space.png");
	spacePanel->GetComponent<Panel>()->SetSortLayer(0);
	spacePanel->SetActive(false);
	AddGameObject(spacePanel);
	EventMachine::GetInstance()->SetSpacePanel(spacePanel);


	shared_ptr<GameObject> fadeOut = make_shared<GameObject>();
	fadeOut->SetName("FadeOut");
	fadeOut->AddComponent<Transform>();
	fadeOut->AddComponent<Panel>();
	fadeOut->GetComponent<Panel>()->SetOffset(0.f, GameProcess::_windowInfo.width, 0.f, GameProcess::_windowInfo.height);
	fadeOut->GetComponent<Panel>()->SetImages(L"black.png");
	fadeOut->GetComponent<Panel>()->SetIsAlpha(true);
	fadeOut->GetComponent<Panel>()->SetAlpha(0.f);
	fadeOut->GetComponent<Panel>()->SetSortLayer(10);
	AddGameObject(fadeOut);
	EventMachine::GetInstance()->SetFadeInOutPanel(fadeOut);

	// 메뉴 패널
	shared_ptr<GameObject> menuPanel = make_shared<GameObject>();
	shared_ptr<GameObject> returnBtn = make_shared<GameObject>();
	shared_ptr<GameObject> newGameBtn = make_shared<GameObject>();
	shared_ptr<GameObject> titleBtn = make_shared<GameObject>();
	shared_ptr<GameObject> optionBtn = make_shared<GameObject>();
	shared_ptr<GameObject> gameExitBtn = make_shared<GameObject>();
	menuPanel->SetName("MenuPanel");
	menuPanel->AddComponent<Transform>();
	menuPanel->AddComponent<Panel>();
	menuPanel->GetComponent<Panel>()->SetOffset(
		GameProcess::_windowInfo.width / 2 - 600.f,
		GameProcess::_windowInfo.width / 2 + 600.f,
		GameProcess::_windowInfo.height / 2 - 450.f,
		GameProcess::_windowInfo.height / 2 + 350.f);
	menuPanel->GetComponent<Panel>()->SetImages(L"Menu.png");
	menuPanel->GetComponent<Panel>()->SetSortLayer(2);
	menuPanel->SetActive(false);
	menuPanel->SetChild(returnBtn);
	menuPanel->SetChild(newGameBtn);
	menuPanel->SetChild(titleBtn);
	menuPanel->SetChild(optionBtn);
	menuPanel->SetChild(gameExitBtn);
	AddGameObject(menuPanel);

	// 돌아가기 버튼
	returnBtn->SetName("ReturnBtn");
	returnBtn->AddComponent<Transform>();
	returnBtn->AddComponent<Button>();
	returnBtn->GetComponent<Button>()->SetSortLayer(3);
	returnBtn->GetComponent<Button>()->SetOffset(GameProcess::_windowInfo.width / 2 - 200.f,
		GameProcess::_windowInfo.width / 2 + 200.f,
		GameProcess::_windowInfo.height / 2 - 240.f,
		GameProcess::_windowInfo.height / 2 - 160.f);
	returnBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::NORMAL, L"Back to.png");
	returnBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::HIGHLIGHT, L"Back to_color.png");
	returnBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::PRESSED, L"Back to_color.png");
	returnBtn->SetActive(false);
	AddGameObject(returnBtn);

	// 새 게임 버튼
	newGameBtn->SetName("NewGameBtn");
	newGameBtn->AddComponent<Transform>();
	newGameBtn->AddComponent<Button>();
	newGameBtn->GetComponent<Button>()->SetSortLayer(3);
	newGameBtn->GetComponent<Button>()->SetOffset(GameProcess::_windowInfo.width / 2 - 200.f,
		GameProcess::_windowInfo.width / 2 + 200.f,
		GameProcess::_windowInfo.height / 2 - 130.f,
		GameProcess::_windowInfo.height / 2 - 50.f);
	newGameBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::NORMAL, L"new game.png");
	newGameBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::HIGHLIGHT, L"new game_color.png");
	newGameBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::PRESSED, L"new game_color.png");
	newGameBtn->SetActive(false);
	AddGameObject(newGameBtn);

	// 타이틀로 버튼
	titleBtn->SetName("TitleBtn");
	titleBtn->AddComponent<Transform>();
	titleBtn->AddComponent<Button>();
	titleBtn->GetComponent<Button>()->SetSortLayer(3);
	titleBtn->GetComponent<Button>()->SetOffset(GameProcess::_windowInfo.width / 2 - 200.f,
		GameProcess::_windowInfo.width / 2 + 200.f,
		GameProcess::_windowInfo.height / 2 - 20.f,
		GameProcess::_windowInfo.height / 2 + 60.f);
	titleBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::NORMAL, L"main screen.png");
	titleBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::HIGHLIGHT, L"main screen_color.png");
	titleBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::PRESSED, L"main screen_color.png");
	titleBtn->SetActive(false);
	AddGameObject(titleBtn);

	// 옵션 버튼
	optionBtn->SetName("OptionBtn");
	optionBtn->AddComponent<Transform>();
	optionBtn->AddComponent<Button>();
	optionBtn->GetComponent<Button>()->SetSortLayer(3);
	optionBtn->GetComponent<Button>()->SetOffset(GameProcess::_windowInfo.width / 2 - 200.f,
		GameProcess::_windowInfo.width / 2 + 200.f,
		GameProcess::_windowInfo.height / 2 + 90.f,
		GameProcess::_windowInfo.height / 2 + 170.f);
	optionBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::NORMAL, L"setting.png");
	optionBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::HIGHLIGHT, L"setting_color.png");
	optionBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::PRESSED, L"setting_color.png");
	optionBtn->SetActive(false);
	AddGameObject(optionBtn);

	// 게임 종료 버튼
	gameExitBtn->SetName("GameExitBtn");
	gameExitBtn->AddComponent<Transform>();
	gameExitBtn->AddComponent<Button>();
	gameExitBtn->GetComponent<Button>()->SetSortLayer(3);
	gameExitBtn->GetComponent<Button>()->SetOffset(GameProcess::_windowInfo.width / 2 - 200.f,
		GameProcess::_windowInfo.width / 2 + 200.f,
		GameProcess::_windowInfo.height / 2 + 200.f,
		GameProcess::_windowInfo.height / 2 + 280.f);
	gameExitBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::NORMAL, L"Game over.png");
	gameExitBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::HIGHLIGHT, L"Game over_color.png");
	gameExitBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::PRESSED, L"Game over_color.png");
	gameExitBtn->SetActive(false);
	AddGameObject(gameExitBtn);

	// 옵션 을 눌렀을때 맨처음 나오는 setting 패널
	shared_ptr<GameObject> settingPanel = make_shared<GameObject>();
	shared_ptr<GameObject> backBtn = make_shared<GameObject>();
	shared_ptr<GameObject> nextBtn = make_shared<GameObject>();
	settingPanel->SetName("SettingPanel");
	settingPanel->AddComponent<Transform>();
	settingPanel->AddComponent<Panel>();
	settingPanel->GetComponent<Panel>()->SetSortLayer(4);
	settingPanel->GetComponent<Panel>()->SetOffset(
		0.f,
		GameProcess::_windowInfo.width,
		0.f,
		GameProcess::_windowInfo.height);
	settingPanel->GetComponent<Panel>()->SetImages(L"setting_1.png");
	settingPanel->SetActive(false);
	settingPanel->SetChild(nextBtn);
	AddGameObject(settingPanel);

	// setting 패널에서 next 버튼 누르면 나오는 패널
	shared_ptr<GameObject> controlPanel = make_shared<GameObject>();
	controlPanel->SetName("ControlPanel");
	controlPanel->AddComponent<Transform>();
	controlPanel->AddComponent<Panel>();
	controlPanel->GetComponent<Panel>()->SetSortLayer(4);
	controlPanel->GetComponent<Panel>()->SetOffset(
		0.f,
		GameProcess::_windowInfo.width,
		0.f,
		GameProcess::_windowInfo.height);
	controlPanel->GetComponent<Panel>()->SetImages(L"Key Settings.png");
	controlPanel->SetActive(false);
	controlPanel->SetChild(backBtn);
	AddGameObject(controlPanel);

	// setting 패널에서의 next 버튼
	nextBtn->SetName("nextBtn");
	nextBtn->AddComponent<Transform>();
	nextBtn->AddComponent<Button>();
	nextBtn->GetComponent<Button>()->SetSortLayer(5);
	nextBtn->GetComponent<Button>()->SetOffset(
		GameProcess::_windowInfo.width / 2 - 300.f,
		GameProcess::_windowInfo.width / 2 - 100.f,
		GameProcess::_windowInfo.height / 2 + 300.f,
		GameProcess::_windowInfo.height / 2 + 380.f);
	nextBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::NORMAL, L"next.png");
	nextBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::HIGHLIGHT, L"next.png");
	nextBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::PRESSED, L"next.png");
	nextBtn->SetActive(false);
	AddGameObject(nextBtn);

	// 컨트롤 패널에서의 back
	backBtn->SetName("backBtn");
	backBtn->AddComponent<Transform>();
	backBtn->AddComponent<Button>();
	backBtn->GetComponent<Button>()->SetSortLayer(5);
	backBtn->GetComponent<Button>()->SetOffset(
		GameProcess::_windowInfo.width / 2 + 100.f,
		GameProcess::_windowInfo.width / 2 + 300.f,
		GameProcess::_windowInfo.height / 2 + 300.f,
		GameProcess::_windowInfo.height / 2 + 380.f);
	backBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::NORMAL, L"back.png");
	backBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::HIGHLIGHT, L"back.png");
	backBtn->GetComponent<Button>()->SetImages((int)ButtonStatus::PRESSED, L"back.png");
	backBtn->SetActive(false);
	AddGameObject(backBtn);

	// ui 매니저
	shared_ptr<GameObject> uiManager = make_shared<GameObject>();
	uiManager->SetName("UIManager");
	uiManager->AddComponent<Transform>();
	uiManager->AddComponent<UIManager>();
	uiManager->GetScript<UIManager>()->SetMenuPanel(menuPanel);
	uiManager->GetScript<UIManager>()->SetReturnBtn(returnBtn);
	uiManager->GetScript<UIManager>()->SetNewGameBtn(newGameBtn);
	uiManager->GetScript<UIManager>()->SetTitleBtn(titleBtn);
	uiManager->GetScript<UIManager>()->SetOptionBtn(optionBtn);
	uiManager->GetScript<UIManager>()->SetGameExitBtn(gameExitBtn);
	uiManager->GetScript<UIManager>()->SetSettingPanel(settingPanel);
	uiManager->GetScript<UIManager>()->SetControlPanel(controlPanel);
	uiManager->GetScript<UIManager>()->SetSettingBtn(backBtn);
	uiManager->GetScript<UIManager>()->SetControlBtn(nextBtn);
	uiManager->GetScript<UIManager>()->SetHeartGauge(heartGauge);
	AddGameObject(uiManager);

	/*shared_ptr<GameObject> soundManager = make_shared<GameObject>();
	soundManager->SetName("SoundManager");
	soundManager->AddComponent<Transform>();
	soundManager->AddComponent<Audio>();
	soundManager->AddComponent<SoundManager>();
	soundManager->GetScript<SoundManager>()->SetMenuPanel(menuPanel);
	soundManager->GetScript<SoundManager>()->SetReturnBtn(returnBtn);
	soundManager->GetScript<SoundManager>()->SetNewGameBtn(newGameBtn);
	soundManager->GetScript<SoundManager>()->SetTitleBtn(titleBtn);
	soundManager->GetScript<SoundManager>()->SetOptionBtn(optionBtn);
	soundManager->GetScript<SoundManager>()->SetGameExitBtn(gameExitBtn);
	soundManager->GetScript<SoundManager>()->SetSettingPanel(settingPanel);
	soundManager->GetScript<SoundManager>()->SetControlPanel(controlPanel);
	soundManager->GetScript<SoundManager>()->SetSettingBtn(backBtn);
	soundManager->GetScript<SoundManager>()->SetControlBtn(nextBtn);

	AddGameObject(soundManager);*/

	ResizeWindow(GameProcess::resizeInfo.width, GameProcess::resizeInfo.height);
}
