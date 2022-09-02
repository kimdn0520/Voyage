#include "pch.h"
#include "BigBoatPrefab.h"
#include "DataManager.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "MonoBehaviour.h"
#include "GameObject.h"
#include "Audio.h"
#include "Panel.h"
#include "GameProcess.h"

BigBoatPrefab::BigBoatPrefab(Vector3 pos)
	: IPrefab(make_shared<GameObject>()),
	fishingGaugeBack(make_shared<GameObject>()), fishingGauge(make_shared<GameObject>()), fishingRange(make_shared<GameObject>())
{
	vector<shared_ptr<GameObject>> bigBoatObjects = DataManager::GetInstance()->GetDataResources("BigBoat");
	bigBoatObjects[0]->GetTransform()->SetLocalRotation(Vector3(0.f, 180.f, 0.f));

	gameObject->SetTag(Tag::Default);
	gameObject->SetName("BigBoat");
	gameObject->AddComponent<Transform>();
	gameObject->AddComponent<Audio>();
	gameObject->GetComponent<Transform>()->SetLocalScale(Vector3(3.f, 3.f, 3.f));
	gameObject->GetComponent<Transform>()->SetLocalPosition(pos);
	gameObject->AddComponent<SphereCollider>();
	gameObject->GetComponent<SphereCollider>()->SetColliderInfo(Vector3(0.f, 0.f, 0.f), 20.f);
	gameObject->GetComponent<SphereCollider>()->SetIsTrigger(true);

	fishingRange->SetName("FishingRange");
	fishingRange->SetActive(false);
	fishingRange->AddComponent<Transform>();
	fishingRange->AddComponent<Panel>();
	fishingRange->GetComponent<Panel>()->SetOffset(
		GameProcess::_windowInfo.width / 2 - 570.f,
		GameProcess::_windowInfo.width / 2 + 530.f,
		GameProcess::_windowInfo.height / 2 - 50.f,
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

	for (auto& gameObj : bigBoatObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(gameObject->GetTransform());
			gameObject->SetChild(gameObj);
		}
	}
}

BigBoatPrefab::~BigBoatPrefab()
{}
