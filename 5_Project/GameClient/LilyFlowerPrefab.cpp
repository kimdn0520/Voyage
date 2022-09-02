#include "pch.h"
#include "LilyFlowerPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"

LilyFlowerPrefab::LilyFlowerPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> lilyFlowerObjects = DataManager::GetInstance()->GetDataResources("LilyFlower");

	gameObject->SetTag(Tag::Default);
	gameObject->SetName("LilyFlower");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(-50.f, 0.f, 0.f));
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, 0.f), 3.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);

	for (auto& gameObj : lilyFlowerObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

LilyFlowerPrefab::~LilyFlowerPrefab()
{}
