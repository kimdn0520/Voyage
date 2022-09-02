#pragma once
#include "IPrefab.h"

class MoneyPrefab : public IPrefab
{
public:
	MoneyPrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~MoneyPrefab();
};

