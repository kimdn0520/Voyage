#pragma once
#include "IPrefab.h"

class BoatPrefab : public IPrefab
{
public:
	BoatPrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~BoatPrefab();

private:
	float _resizeWidth;
	float _resizeHeight;

public:
	shared_ptr<GameObject> fishingGaugeBack;

	shared_ptr<GameObject> fishingGauge;

	shared_ptr<GameObject> fishingRange;
};

