#include "pch.h"
#include "MoneyPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"
#include "Item.h"

MoneyPrefab::MoneyPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> moneyObjects = DataManager::GetInstance()->GetDataResources("Money");

	gameObject->SetTag(Tag::Item);
	gameObject->SetName("Money");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale({ 4.5f, 4.5f, 4.5f });
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(-20.f, 0.f, 0.f));
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, 0.f), 4.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	gameObject->AddComponent<Item>();
	gameObject->GetScript<Item>()->SetType(ItemType::Money);


	for (auto& gameObj : moneyObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

MoneyPrefab::~MoneyPrefab()
{}
