#include "pch.h"
#include "TestScene.h"
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

void TestScene::Init()
{
	if (!DataManager::GetInstance()->isDataLoad)
		DataManager::GetInstance()->DataLoad();

	//---------------------------------------------------------------------------------------------------------
	// 메인 카메라
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->SetName("MainCamera");
	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraScript>();
	camera->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 20.f, 0.f));
	camera->GetComponent<Transform>()->LookAt(Vector3(0.f, 0.f, 0.f));
	/*camera->AddComponent<Light>();
	camera->GetComponent<Light>()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
	camera->GetComponent<Light>()->SetLightDirection(camera->GetComponent<Transform>()->GetLook());
	camera->GetComponent<Light>()->SetDiffuse(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	camera->GetComponent<Light>()->SetAmbient(Vector4(0.2f, 0.2f, 0.2f, 1.0f));
	camera->GetComponent<Light>()->SetSpecular(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	camera->GetComponent<Light>()->SetATT(Vector3(1.0f, 0.f, 0.f));
	camera->GetComponent<Light>()->SetSpot(90.f);
	camera->GetComponent<Light>()->SetRange(10000.f);
	camera->GetComponent<Light>()->SendLightInfo();*/
	AddGameObject(camera);

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
	Resources::GetInstance()->SetCubeMapTexture(skyBox->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"grasscube.dds");
	AddGameObject(skyBox);

	//---------------------------------------------------------------------------------------------------------
	// 텍스쳐 큐브
	//---------------------------------------------------------------------------------------------------------
	/*shared_ptr<GameObject> exBox = make_shared<GameObject>();
	exBox->SetName("exBox");
	exBox->AddComponent<Transform>();
	exBox->AddComponent<MeshRenderer>();
	exBox->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, -10.f, 0.f));
	exBox->GetComponent<Transform>()->SetLocalScale(Vector3(300.f, 10.f, 300.f));
	exBox->AddComponent<BoxCollider>();
	exBox->GetComponent<BoxCollider>()->SetColliderInfo(Vector3(0.f, 0.f, 0.f), Vector3(300.f, 10.f, 300.f));
	exBox->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::CubeMesh);
	exBox->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = DataManager::GetInstance()->GetDiffuseTextureObjId();
	exBox->GetComponent<MeshRenderer>()->GetMeshInfo()->type = OBJECT_TYPE::WATER;
	Resources::GetInstance()->SetDiffuseMapTexture(water->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"WoodCrate.dds");
	Resources::GetInstance()->SetNormalMapTexture(water->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"water01.dds");
	Resources::GetInstance()->SetCubeMapTexture(water->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"grasscube.dds");
	AddGameObject(exBox);*/

	//---------------------------------------------------------------------------------------------------------
	// 텍스쳐 큐브2
	//---------------------------------------------------------------------------------------------------------
	/*shared_ptr<GameObject> exBox2 = make_shared<GameObject>();
	exBox2->SetName("exBox2");
	exBox2->AddComponent<Transform>();
	exBox2->AddComponent<MeshRenderer>();
	exBox2->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.f, 80.f));
	exBox2->GetComponent<Transform>()->SetLocalScale(Vector3(5.f, 5.f, 5.f));
	exBox2->AddComponent<SphereCollider>();
	exBox2->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 5.f, 0.f), 5.f);
	exBox2->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::CubeMesh);
	exBox2->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = DataManager::GetInstance()->GetSkyBoxId();
	AddGameObject(exBox2);*/
	
	//---------------------------------------------------------------------------------------------------------
	// Axis
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> axis = make_shared<GameObject>();
	axis->SetName("axis");
	axis->AddComponent<Transform>();
	axis->AddComponent<MeshRenderer>();
	axis->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::AxisMesh);
	axis->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = DataManager::GetInstance()->GetSkyBoxId();
	AddGameObject(axis);

	//---------------------------------------------------------------------------------------------------------
	// Grid
	//---------------------------------------------------------------------------------------------------------
	/*shared_ptr<GameObject> grid = make_shared<GameObject>();
	grid->SetName("grid");
	grid->AddComponent<Transform>();
	grid->AddComponent<MeshRenderer>();
	grid->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::GridMesh);
	grid->GetComponent<MeshRenderer>()->PushMaterial();
	grid->GetComponent<MeshRenderer>()->GetMeshInfo()->materials[0]->isUtil = true;
	AddGameObject(grid);*/


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
	dirLight->GetComponent<Light>()->SetDiffuse(Vector4(0.5f, 0.5f, 0.5f, 1.0f));		// 기본0.5 
	dirLight->GetComponent<Light>()->SetAmbient(Vector4(0.6f, 0.6f, 0.6f, 1.0f));		// 기본0.2
	dirLight->GetComponent<Light>()->SetSpecular(Vector4(0.5f, 0.5f, 0.5f, 1.0f));   // 기본0.5
	dirLight->GetComponent<Light>()->SendLightInfo();
	AddGameObject(dirLight);

	//---------------------------------------------------------------------------------------------------------
	// ASE Model - example
	//---------------------------------------------------------------------------------------------------------
	vector<shared_ptr<GameObject>> starObjects = DataManager::GetInstance()->GetDataResources("Star");
	shared_ptr<GameObject> star = starObjects[0];
	star->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
	star->AddComponent<SphereCollider>();
	star->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 5.f, 0.f), 10.f);
	star->GetComponent<SphereCollider>()->SetIsTrigger(true);
	star->AddComponent<Player>();
	star->GetScript<Player>()->SetMainCamera(camera);
	for(auto& gameObj : starObjects)
		AddGameObject(gameObj);
	camera->GetScript<CameraScript>()->_targetTransfom = star->GetComponent<Transform>();
	
	vector<shared_ptr<GameObject>> adultGirlObjects = DataManager::GetInstance()->GetDataResources("AdultGirl");
	shared_ptr<GameObject> adultGirl = adultGirlObjects[0];
	adultGirl->GetComponent<Transform>()->SetLocalPosition(Vector3(5.f, 0.f, 0.f));
	adultGirl->AddComponent<Player>();
	adultGirl->GetScript<Player>()->SetMainCamera(camera);
	for (auto& gameObj : adultGirlObjects)
		AddGameObject(gameObj);

	vector<shared_ptr<GameObject>> adultBoyObjects = DataManager::GetInstance()->GetDataResources("AdultBoy");
	shared_ptr<GameObject> adultBoy = adultBoyObjects[0];
	adultBoy->GetComponent<Transform>()->SetLocalPosition(Vector3(10.f, 0.f, 0.f));
	for (auto& gameObj : adultBoyObjects)
		AddGameObject(gameObj);

	vector<shared_ptr<GameObject>> adultBoyObjects2 = DataManager::GetInstance()->GetDataResources("AdultBoy");
	shared_ptr<GameObject> adultBoy2 = adultBoyObjects2[0];
	adultBoy2->GetComponent<Transform>()->SetLocalPosition(Vector3(15.f, 0.f, 0.f));
	for (auto& gameObj : adultBoyObjects2)
		AddGameObject(gameObj);

	vector<shared_ptr<GameObject>> clockObjects = DataManager::GetInstance()->GetDataResources("Clock");
	shared_ptr<GameObject> clock = clockObjects[0];
	clock->GetComponent<Transform>()->SetLocalPosition(Vector3(20.f, 0.f, 0.f));
	for (auto& gameObj : clockObjects)
		AddGameObject(gameObj);

	vector<shared_ptr<GameObject>> flowerObjects = DataManager::GetInstance()->GetDataResources("LilyFlower");
	shared_ptr<GameObject> flower = flowerObjects[0];
	flower->GetComponent<Transform>()->SetLocalPosition(Vector3(25.f, 0.f, 0.f));
	for (auto& gameObj : flowerObjects)
		AddGameObject(gameObj);

	vector<shared_ptr<GameObject>> heartObjects = DataManager::GetInstance()->GetDataResources("Heart");
	shared_ptr<GameObject> heart = heartObjects[0];
	heart->GetComponent<Transform>()->SetLocalPosition(Vector3(30.f, 0.f, 0.f));
	for (auto& gameObj : heartObjects)
		AddGameObject(gameObj);

	shared_ptr<GameObject> testText = make_shared<GameObject>();
	testText->SetName("testText");
	testText->AddComponent<Text>();
	testText->AddComponent<TextScript>();
	testText->GetScript<TextScript>()->SetTransform(star->GetTransform());
	AddGameObject(testText);

	/*shared_ptr<GameObject> circle = make_shared<GameObject>();
	circle->SetName("circle");
	circle->AddComponent<Transform>();
	circle->AddComponent<MeshRenderer>();
	circle->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::CircleMesh);
	circle->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = DataManager::GetInstance()->GetDefaultID();
	AddGameObject(circle);*/

	shared_ptr<GameObject> btn1 = make_shared<GameObject>();
	btn1->SetName("testBtn");
	btn1->AddComponent<Transform>();
	btn1->AddComponent<Button>();
	btn1->GetComponent<Button>()->SetOffset(100.f, 200.f, 100.f, 200.f);	// 왼쪽위를 0,0 으로 보면됩니다.
	btn1->GetComponent<Button>()->SetImages((int)ButtonStatus::NORMAL, L"foam.dds");
	btn1->GetComponent<Button>()->SetImages((int)ButtonStatus::HIGHLIGHT, L"boat.dds");
	btn1->GetComponent<Button>()->SetImages((int)ButtonStatus::PRESSED, L"water_height.dds");
	AddGameObject(btn1);
}