#include "pch.h"
#include "BirdDiePrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"

BirdDiePrefab::BirdDiePrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> birdDieObjects = DataManager::GetInstance()->GetDataResources("BirdDie");

	gameObject->SetTag(Tag::Npc);
	gameObject->SetName("BirdDie");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(10.f, 10.f, 10.f));
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);

	for (auto& gameObj : birdDieObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

BirdDiePrefab::~BirdDiePrefab()
{}
