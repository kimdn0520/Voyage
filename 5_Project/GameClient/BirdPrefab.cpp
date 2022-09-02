#include "pch.h"
#include "BirdPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"

BirdPrefab::BirdPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> birdObjects = DataManager::GetInstance()->GetDataResources("Bird");

	gameObject->SetTag(Tag::Npc);
	gameObject->SetName("Bird");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(10.f, 10.f, 10.f));
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(30.f, -15.f, 0.f));
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);

	for (auto& gameObj : birdObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

BirdPrefab::~BirdPrefab()
{}
