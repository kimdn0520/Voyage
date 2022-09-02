#include "pch.h"
#include "SmallBoatPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"

SmallBoatPrefab::SmallBoatPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> smallBoatObjects = DataManager::GetInstance()->GetDataResources("SmallBoat");

	gameObject->SetTag(Tag::Player);
	gameObject->SetName("SmallBoat");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale({ 1.2f, 1.2f, 1.2f });
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 5.f, 0.f), 10.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	gameObject->AddComponent<Audio>();

	for (auto& gameObj : smallBoatObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

SmallBoatPrefab::~SmallBoatPrefab()
{}
