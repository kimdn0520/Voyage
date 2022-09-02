#include "pch.h"
#include "StarPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Audio.h"
#include "Item.h"

StarPrefab::StarPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> startObjects = DataManager::GetInstance()->GetDataResources("Star");

	for (auto& star : startObjects)
	{
		star->GetComponent<MeshRenderer>()->GetMeshInfo()->isBloom = true;
	}

	gameObject->SetTag(Tag::Item);
	gameObject->SetName("Star");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale({ 4.f, 4.f, 4.f });
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(-20.f, 0.f, 0.f));
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, 0.f), 4.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	gameObject->AddComponent<Item>();
	gameObject->GetScript<Item>()->SetType(ItemType::Star);

	for (auto& gameObj : startObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

StarPrefab::~StarPrefab()
{}
