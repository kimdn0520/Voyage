#include "pch.h"
#include "GrandFaPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"
#include "Panel.h"
#include "GameProcess.h"
#include "MeshRenderer.h"
#include "NewbieEngineFunc.h"

GrandFaPrefab::GrandFaPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> grandfaObjects = DataManager::GetInstance()->GetDataResources("Grandfa");

	gameObject->SetTag(Tag::Player);
	gameObject->SetName("GrandFa");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));

	for (auto& gameObj : grandfaObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

GrandFaPrefab::~GrandFaPrefab()
{}
