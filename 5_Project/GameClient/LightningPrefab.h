#pragma once
#include "IPrefab.h"

class LightningPrefab : public IPrefab
{
public:
	LightningPrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~LightningPrefab();

private:
};

