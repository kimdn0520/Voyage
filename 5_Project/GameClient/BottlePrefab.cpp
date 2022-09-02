#include "pch.h"
#include "BottlePrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"
#include "Item.h"

BottlePrefab::BottlePrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> bottleObjects = DataManager::GetInstance()->GetDataResources("Bottle");

	gameObject->SetTag(Tag::Item);
	gameObject->SetName("Bottle");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(-130.f, 0.f, 0.f));
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 2.f, 0.f), 4.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	gameObject->AddComponent<Item>();
	gameObject->GetScript<Item>()->SetType(ItemType::Bottle);

	for (auto& gameObj : bottleObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

BottlePrefab::~BottlePrefab()
{}
