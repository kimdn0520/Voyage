#include "pch.h"
#include "ClockPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"

ClockPrefab::ClockPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> startObjects = DataManager::GetInstance()->GetDataResources("Clock");

	gameObject->SetTag(Tag::Item);
	gameObject->SetName("Clock");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, 0.f), 3.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);

	for (auto& gameObj : startObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

ClockPrefab::~ClockPrefab()
{}
