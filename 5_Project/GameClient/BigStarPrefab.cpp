#include "pch.h"
#include "BigStarPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Audio.h"
#include "Item.h"
#include "Light.h"

BigStarPrefab::BigStarPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> bigStartObjects = DataManager::GetInstance()->GetDataResources("BigStar");
	
	for (auto& star : bigStartObjects)
	{
		star->GetComponent<MeshRenderer>()->GetMeshInfo()->isBloom = true;
	}

	gameObject->SetTag(Tag::Item);
	gameObject->SetName("BigStar");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale({ 6.f, 6.f, 6.f });
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(-20.f, 0.f, 0.f));
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, 0.f), 7.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	/*gameObject->AddComponent<Light>();
	gameObject->GetComponent<Light>()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
	gameObject->GetComponent<Light>()->SetDiffuse(Vector4(0.5f, 0.5f, 0.5f, 1.0f));		 
	gameObject->GetComponent<Light>()->SetAmbient(Vector4(0.6f, 0.6f, 0.6f, 1.0f));		
	gameObject->GetComponent<Light>()->SetSpecular(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	gameObject->GetComponent<Light>()->SetATT(Vector3(1.0f, 0.f, 0.f));
	gameObject->GetComponent<Light>()->SetRange(20.f);
	gameObject->GetComponent<Light>()->SendLightInfo();*/
	gameObject->AddComponent<Item>();
	gameObject->GetScript<Item>()->SetType(ItemType::BigStar);

	for (auto& gameObj : bigStartObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

BigStarPrefab::~BigStarPrefab()
{}
