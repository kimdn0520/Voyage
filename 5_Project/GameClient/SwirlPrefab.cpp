#include "pch.h"
#include "SwirlPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"
#include "Item.h"
#include "Swirl.h"

SwirlPrefab::SwirlPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> swirlObjects = DataManager::GetInstance()->GetDataResources("Swirl");

	gameObject->SetTag(Tag::Disaster);
	gameObject->SetName("Swirl");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale({ 10.f, 10.f, 10.f });
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 0.f, 180.f));
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 10.f, 0.f), 16.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	gameObject->AddComponent<Swirl>();

	for (auto& gameObj : swirlObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

SwirlPrefab::~SwirlPrefab()
{}
