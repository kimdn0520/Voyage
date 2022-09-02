#include "pch.h"
#include "IslandPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"
#include "Ground.h"

IslandPrefab::IslandPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> islandObjects = DataManager::GetInstance()->GetDataResources("Island");

	gameObject->SetTag(Tag::Ground);
	gameObject->SetName("Island");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(4.f, 4.f, 4.f));
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));
	gameObject->AddComponent<BoxCollider>();
	gameObject->AddComponent<Ground>();
	gameObject->GetComponent<BoxCollider>()->SetColliderInfo(Vector3(4.f, 4.f, 4.f), Vector3(300.f, 5.f , 20.f));
	gameObject->GetComponent<BoxCollider>()->SetIsTrigger(true);

	for (auto& gameObj : islandObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

IslandPrefab::~IslandPrefab()
{}
