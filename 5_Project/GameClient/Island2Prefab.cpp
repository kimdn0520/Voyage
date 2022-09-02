#include "pch.h"
#include "Island2Prefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"
#include "Ground.h"

Island2Prefab::Island2Prefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> island2Objects = DataManager::GetInstance()->GetDataResources("Island2");

	gameObject->SetTag(Tag::Ground2);
	gameObject->SetName("Island2");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(3.f, 3.f, 3.f));
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));
	gameObject->AddComponent<BoxCollider>();
	gameObject->AddComponent<Ground>();
	gameObject->GetComponent<BoxCollider>()->SetColliderInfo(Vector3(4.f, 4.f, 4.f), Vector3(300.f, 5.f, 20.f));
	gameObject->GetComponent<BoxCollider>()->SetIsTrigger(true);

	for (auto& gameObj : island2Objects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

Island2Prefab::~Island2Prefab()
{}
