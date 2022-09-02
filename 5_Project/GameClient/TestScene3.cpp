#include "pch.h"
#include "TestScene3.h"
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
#include "Audio.h"
#include "AudioListener.h"

void TestScene3::Init()
{
	//---------------------------------------------------------------------------------------------------------
	// 메인 카메라
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->SetName("MainCamera");
	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraScript>();
	camera->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 10.f, -30.f));
	camera->GetComponent<Transform>()->LookAt(Vector3(0.f, 10.f, 0.f));

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
	Resources::GetInstance()->SetCubeMapTexture(skyBox->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"grasscube");
	AddGameObject(skyBox);


	//---------------------------------------------------------------------------------------------------------
	// 텍스쳐 큐브
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> exBox = make_shared<GameObject>();
	exBox->SetName("exBox");
	exBox->AddComponent<Transform>();
	exBox->AddComponent<MeshRenderer>();
	exBox->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.f, 50.f));
	exBox->GetComponent<Transform>()->SetLocalScale(Vector3(300.f, 0.f, 300.f));
	exBox->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::CubeMesh);
	exBox->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = DataManager::GetInstance()->GetDiffuseTextureObjId();
	Resources::GetInstance()->SetDiffuseMapTexture(exBox->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"WoodCrate");
	AddGameObject(exBox);

	shared_ptr<GameObject> dirLight = make_shared<GameObject>();
	dirLight->SetName("dirLight");
	dirLight->AddComponent<Transform>();
	dirLight->AddComponent<Light>();
	dirLight->GetComponent<Transform>()->SetLocalPosition(Vector3(20000.f, 20000.f, 20000.f));
	dirLight->GetComponent<Light>()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
	dirLight->GetComponent<Light>()->SetLightDirection(Vector3(0.57735f, -0.57735f, 0.57735f));
	dirLight->GetComponent<Light>()->SetDiffuse(Vector4(0.5f, 0.5f, 0.5f, 1.0f));		// 기본0.5 
	dirLight->GetComponent<Light>()->SetAmbient(Vector4(0.6f, 0.6f, 0.6f, 1.0f));		// 기본0.2
	dirLight->GetComponent<Light>()->SetSpecular(Vector4(0.05f, 0.05f, 0.05f, 1.0f));   // 기본0.5
	dirLight->GetComponent<Light>()->SendLightInfo();
	AddGameObject(dirLight);


	//---------------------------------------------------------------------------------------------------------
	// Sound - example
	//---------------------------------------------------------------------------------------------------------
	shared_ptr<GameObject> _Audio = make_shared<GameObject>();

	_Audio->SetName("audio");
	_Audio->AddComponent<Audio>();
	/*_Audio->GetComponent<Audio>()->PlayEvent("even:/BGM_Main ");*/
	_Audio->GetComponent<Audio>()->PlayEvent("event:/AMB & BGM/BGM_Main");


}
