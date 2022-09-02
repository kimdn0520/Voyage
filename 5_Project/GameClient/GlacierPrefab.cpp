#include "pch.h"
#include "GlacierPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"

GlacierPrefab::GlacierPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> glacierObjects = DataManager::GetInstance()->GetDataResources("Glacier");

	gameObject->SetTag(Tag::Item);
	gameObject->SetName("Glacier");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(5.f, 5.f, 5.f));
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
	/*gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, 0.f), 4.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);*/

	for (auto& gameObj : glacierObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

GlacierPrefab::~GlacierPrefab()
{}
