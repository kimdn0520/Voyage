#include "pch.h"
#include "AdultBoyPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"

AdultBoyPrefab::AdultBoyPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> adultBoyObjects = DataManager::GetInstance()->GetDataResources("AdultBoy");

	gameObject->SetTag(Tag::Npc);
	gameObject->SetName("AdultBoy");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));

	for (auto& gameObj : adultBoyObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

AdultBoyPrefab::~AdultBoyPrefab()
{}
