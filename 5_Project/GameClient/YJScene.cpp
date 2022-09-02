#include "pch.h"
#include "YJScene.h"
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
#include "DataManager.h"

void YJScene::Init()
{
	// 임시
	DataManager::GetInstance()->DataLoad();

	//---------------------------------------------------------------------------------------------------------
	// 메인 카메라
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->SetName("MainCamera");
	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraScript>();
	camera->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 90.f, 0.f));
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
	// Water
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> water = make_shared<GameObject>();
	water->SetName("water");
	water->AddComponent<Transform>();
	water->AddComponent<MeshRenderer>();
	water->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
	water->GetComponent<Transform>()->SetLocalRotation(Vector3(90.f, 0.f, 0.f));
	water->GetComponent<Transform>()->SetLocalScale(Vector3(100.f, 100.f, 0.f));                                                                                                       
	water->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::ScreenMesh);
	water->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = DataManager::GetInstance()->GetOceanID();
	water->GetComponent<MeshRenderer>()->GetMeshInfo()->type = OBJECT_TYPE::WATER;
	Resources::GetInstance()->SetNormalMapTexture(water->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"water_normal.dds");

	AddGameObject(water);

	////---------------------------------------------------------------------------------------------------------
	//// Water
	////---------------------------------------------------------------------------------------------------------
	//shared_ptr<GameObject> water2 = make_shared<GameObject>();
	//water2->SetName("water2");
	//water2->AddComponent<Transform>();
	//water2->AddComponent<MeshRenderer>();
	//water2->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.f, 100.f));
	//water2->GetComponent<Transform>()->SetLocalRotation(Vector3(90.f, 180.f, 0.f));
	//water2->GetComponent<Transform>()->SetLocalScale(Vector3(50.f, 50.f, 0.f));
	//water2->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::ScreenMesh);
	//water2->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = DataManager::GetInstance()->GetOceanID();
	//water2->GetComponent<MeshRenderer>()->GetMeshInfo()->type = OBJECT_TYPE::WATER;
	//AddGameObject(water2);

	vector<shared_ptr<GameObject>> pigObjects = DataManager::GetInstance()->GetDataResources("SmallBoat");
	shared_ptr<GameObject> pig = pigObjects[0];
	//pig->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 90.f, 0.f));
	pig->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 5.f, 0.f));
	/*pig->AddComponent<Player>();
	pig->GetScript<Player>()->SetMainCamera(camera);*/
	for(auto& gameObj : pigObjects)
		AddGameObject(gameObj);
	camera->GetScript<CameraScript>()->_targetTransfom = pig->GetComponent<Transform>();


	//---------------------------------------------------------------------------------------------------------
	// 물을 위한 카메라!
	//---------------------------------------------------------------------------------------------------------
	Vector3 center= Vector3(0.f,0.f,0.f);			// water의 중심
	
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
	//subCam[1]->GetComponent<Transform>()->RotateYAxis(3.141592f);
	subCam[2]->GetComponent<Transform>()->RotateYAxis(3.141592f);

	Vector3 _pigpos = pig->GetComponent<Transform>()->GetLocalPosition();
	_pigpos.y = -150;
	subCam[1]->GetComponent<Transform>()->SetLocalPosition(_pigpos);

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
	dirLight->GetComponent<Light>()->SetDiffuse(Vector4(0.5f, 0.5f, 0.5f, 1.0f));	// 기본0.5 
	dirLight->GetComponent<Light>()->SetAmbient(Vector4(0.6f, 0.6f, 0.6f, 1.0f));	// 기본0.2
	dirLight->GetComponent<Light>()->SetSpecular(Vector4(0.2f, 0.2f, 0.2f, 1.0f));  // 기본0.5
	dirLight->GetComponent<Light>()->SendLightInfo();
	AddGameObject(dirLight);	


}