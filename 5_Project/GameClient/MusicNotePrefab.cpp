#include "pch.h"
#include "MusicNotePrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"
#include "Item.h"

MusicNotePrefab::MusicNotePrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> musicNoteObjects = DataManager::GetInstance()->GetDataResources("MusicNote");

	gameObject->SetTag(Tag::Item);
	gameObject->SetName("MusicNote");
	gameObject->AddComponent<Transform>();
	gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(5.f, 5.f, 5.f));
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->GetComponent<Transform>()->SetLocalRotation(Vector3(-30.f, 0.f, 0.f));
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, 0.f), 4.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);
	gameObject->AddComponent<Item>();
	gameObject->GetScript<Item>()->SetType(ItemType::Note);

	for (auto& gameObj : musicNoteObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

MusicNotePrefab::~MusicNotePrefab()
{}
