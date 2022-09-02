#include "pch.h"
#include "AdultGirlPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"

AdultGirlPrefab::AdultGirlPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> adultGirlObjects = DataManager::GetInstance()->GetDataResources("AdultGirl");

	gameObject->SetTag(Tag::Default);
	gameObject->SetName("AdultGirl");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));

	for (auto& gameObj : adultGirlObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

AdultGirlPrefab::~AdultGirlPrefab()
{}
