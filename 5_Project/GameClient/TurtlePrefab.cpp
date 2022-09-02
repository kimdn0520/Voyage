#include "pch.h"
#include "TurtlePrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"

TurtlePrefab::TurtlePrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> turtleObjects = DataManager::GetInstance()->GetDataResources("Turtle");

	gameObject->SetTag(Tag::Npc);
	gameObject->SetName("Turtle");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(5.f, 5.f, 5.f));
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 0.f, 0.f));
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, 0.f), 6.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);

	for (auto& gameObj : turtleObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

TurtlePrefab::~TurtlePrefab()
{}
