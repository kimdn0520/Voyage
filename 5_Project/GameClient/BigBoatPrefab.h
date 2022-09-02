#pragma once
#include "IPrefab.h"

class BigBoatPrefab : public IPrefab
{
public:
	BigBoatPrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~BigBoatPrefab();

public:
	shared_ptr<GameObject> fishingGaugeBack;

	shared_ptr<GameObject> fishingGauge;

	shared_ptr<GameObject> fishingRange;
};

