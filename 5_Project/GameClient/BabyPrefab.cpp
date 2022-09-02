#include "pch.h"
#include "BabyPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"

BabyPrefab::BabyPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> babyObjects = DataManager::GetInstance()->GetDataResources("Baby");

	gameObject->SetTag(Tag::Npc);
	gameObject->SetName("Baby");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));

	for (auto& gameObj : babyObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

BabyPrefab::~BabyPrefab()
{}
