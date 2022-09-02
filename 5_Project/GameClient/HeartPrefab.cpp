#include "pch.h"
#include "HeartPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "Item.h"

HeartPrefab::HeartPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> heartObjects = DataManager::GetInstance()->GetDataResources("Heart");

	for (auto& heart : heartObjects)
	{
		heart->GetComponent<MeshRenderer>()->GetMeshInfo()->isBloom = true;
	}

	gameObject->SetTag(Tag::Item);
	gameObject->SetName("Heart");					
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale({ 5.f, 5.f, 5.f });
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(-20.f, 0.f, 0.f));
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, 0.f), 4.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	gameObject->AddComponent<Item>();
	gameObject->GetScript<Item>()->SetType(ItemType::Heart);

	for (auto& gameObj : heartObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}	
}

HeartPrefab::~HeartPrefab()
{}
