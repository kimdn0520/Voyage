#include "pch.h"
//#include "Credit.h"
//#include "Transform.h"
//#include "GameObject.h"
//#include "Camera.h"
//#include "CameraScript.h"
//#include "Resources.h"
//#include "MeshRenderer.h"
//#include "Light.h"
//#include "MeshData.h"
//#include "Animator.h"
//#include "Text.h"
//#include "TextScript.h"
//#include "DataManager.h"
//#include "BoxCollider.h"
//#include "Button.h"
//#include "Panel.h"
//#include "Canvas.h"
//#include "UIManager.h"
//#include "GameProcess.h"
//#include "Audio.h"
//#include "EventMachine.h"
//#include "SoundManager.h"
//#include "BoatPrefab.h"
//
//
//void Credit::Init()
//{
//
//	if (!DataManager::GetInstance()->isDataLoad)
//		DataManager::GetInstance()->DataLoad();
//
//	// INTRO_END 에서 isAdulthoodIntro가 true가 되면 다음 이벤트로 넘어감
//	EventMachine::GetInstance()->isYouthIntro = true;
//
//	// 테스트용 임시
//	EventMachine::GetInstance()->ChangeState(EventMachine::YOUTH_INTRO);
//
//	//---------------------------------------------------------------------------------------------------------
//	// 메인 카메라
//	//---------------------------------------------------------------------------------------------------------
//	shared_ptr<GameObject> camera = make_shared<GameObject>();
//	camera->SetName("CreditCamera");
//	camera->AddComponent<Transform>();
//	camera->AddComponent<Camera>();
//	camera->AddComponent<CameraScript>();
//	camera->AddComponent<Audio>();
//	camera->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 150.f, 0.f));
//	camera->GetComponent<Transform>()->LookAt(Vector3(0.f, 0.f, 0.f));
//	AddGameObject(camera);
//	EventMachine::GetInstance()->SetCamera(camera);
//
//	//---------------------------------------------------------------------------------------------------------
//	// SkyBox
//	//---------------------------------------------------------------------------------------------------------
//	shared_ptr<GameObject> skyBox = make_shared<GameObject>();
//	skyBox->SetName("SkyBox");
//	skyBox->AddComponent<Transform>();
//	skyBox->AddComponent<MeshRenderer>();
//	skyBox->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::SphereMesh);
//	skyBox->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = DataManager::GetInstance()->GetSkyBoxId();
//	skyBox->GetComponent<MeshRenderer>()->GetMeshInfo()->type = OBJECT_TYPE::SKY_BOX;
//	Resources::GetInstance()->SetCubeMapTexture(skyBox->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"daylight.dds");
//	AddGameObject(skyBox);
//
//	//---------------------------------------------------------------------------------------------------------
//	// Water 
//	//---------------------------------------------------------------------------------------------------------
//	shared_ptr<GameObject> water = make_shared<GameObject>();
//	water->SetName("water");
//	water->AddComponent<Transform>();
//	water->AddComponent<MeshRenderer>();
//	water->AddComponent<BoxCollider>();
//	water->GetComponent<BoxCollider>()->SetColliderInfo(Vector3(0.f, -10.f, 50.f), Vector3(200.f, 1.f, 200.f));
//	water->GetComponent<BoxCollider>()->SetIsTrigger(true);
//	water->GetComponent<Transform>()->SetLocalPosition(Vector3(0.f, 0.f, 0.f));
//	//water->GetComponent<Transform>()->SetLocalRotation(Vector3(90.f, 0.f, 0.f));
//	water->GetComponent<Transform>()->SetLocalScale(Vector3(165.f, 0.f, 82.f));
//	water->GetComponent<MeshRenderer>()->SetMesh((int)MeshName::RectMesh);
//	water->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = DataManager::GetInstance()->GetOceanID();
//	water->GetComponent<MeshRenderer>()->GetMeshInfo()->type = OBJECT_TYPE::WATER;
//	Resources::GetInstance()->SetNormalMapTexture(water->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID, L"water_normal.dds");
//
//	AddGameObject(water);
//
//	////---------------------------------------------------------------------------------------------------------
//	//// 크래딧을 위한 이미지 생성
//	////---------------------------------------------------------------------------------------------------------
//	shared_ptr<GameObject>CreditImage = make_shared<GameObject>();
//	CreditImage->SetName("CreditImage");
//	CreditImage->AddComponent<Transform>();
//	CreditImage->AddComponent<Panel>();
//	//CreditImage->GetComponent<Panel>()->SetOffset(0.f, GameProcess::_windowInfo.width, 0.f, GameProcess::_windowInfo.height);
//	//CreditImage->GetComponent<Panel>()->SetImages(L"Loading.png");
//	AddGameObject(CreditImage);
//
//
//
//}
