#include "pch.h"
#include "BoatPrefab.h"
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

BoatPrefab::BoatPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>()),
	fishingGaugeBack(make_shared<GameObject>()), fishingGauge(make_shared<GameObject>()), fishingRange(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> boatObjects = DataManager::GetInstance()->GetDataResources("Boat");
	boatObjects[0]->GetTransform()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));

	gameObject->SetTag(Tag::Player);
	gameObject->SetName("Boat");
	gameObject->AddComponent<Transform>();
	gameObject->AddComponent<Audio>();
	gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(4.f, 4.f, 4.f));
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, 0.f), 20.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);

	fishingRange->SetName("FishingRange");
	fishingRange->SetActive(false);
	fishingRange->AddComponent<Transform>();
	fishingRange->AddComponent<Panel>();
	fishingRange->GetComponent<Panel>()->SetOffset(
		GameProcess::_windowInfo.width / 2 - 590.f,
		GameProcess::_windowInfo.width / 2 + 530.f,
		GameProcess::_windowInfo.height / 2 - 70.f,
		GameProcess::_windowInfo.height / 2 + 550.f);
	fishingRange->GetComponent<Panel>()->SetImages(L"Range.png");
	fishingRange->GetComponent<Panel>()->SetSortLayer(0);
	fishingRange->GetComponent<Panel>()->screenResize = Vector2(1920.f, 1080.f);
	fishingRange->GetComponent<Panel>()->Resize(GameProcess::resizeInfo.width, GameProcess::resizeInfo.height);
	gameObject->SetChild(fishingRange); 

	fishingGaugeBack->SetName("FishingGaugeBack");
	fishingGaugeBack->AddComponent<Transform>();
	fishingGaugeBack->AddComponent<Panel>();
	fishingGaugeBack->GetComponent<Panel>()->SetOffset(
		GameProcess::_windowInfo.width / 2 - 50.f,
		GameProcess::_windowInfo.width / 2 + 50.f,
		GameProcess::_windowInfo.height / 2 + 60.f,
		GameProcess::_windowInfo.height / 2 + 80.f);
	fishingGaugeBack->GetComponent<Panel>()->SetImages(L"white.png");
	fishingGaugeBack->GetComponent<Panel>()->SetSortLayer(0);
	fishingGaugeBack->GetComponent<Panel>()->screenResize = Vector2(1920.f, 1080.f);
	fishingGaugeBack->GetComponent<Panel>()->Resize(GameProcess::resizeInfo.width, GameProcess::resizeInfo.height);
	fishingGaugeBack->SetActive(false);
	gameObject->SetChild(fishingGaugeBack);

	fishingGauge->SetName("FishingGauge");
	fishingGauge->AddComponent<Transform>();
	fishingGauge->AddComponent<Panel>();
	fishingGauge->GetComponent<Panel>()->SetOffset(
		GameProcess::_windowInfo.width / 2 - 50.f,
		GameProcess::_windowInfo.width / 2 - 50.f,
		GameProcess::_windowInfo.height / 2 + 60.f,
		GameProcess::_windowInfo.height / 2 + 80.f);
	fishingGauge->GetComponent<Panel>()->SetImages(L"Green.png");
	fishingGauge->GetComponent<Panel>()->SetSortLayer(1);
	fishingGauge->GetComponent<Panel>()->screenResize = Vector2(1920.f, 1080.f);
	fishingGauge->GetComponent<Panel>()->Resize(GameProcess::resizeInfo.width, GameProcess::resizeInfo.height);
	fishingGauge->SetActive(false);
	gameObject->SetChild(fishingGauge);
	
	for (auto& gameObj : boatObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

BoatPrefab::~BoatPrefab()
{}
