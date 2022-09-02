#pragma once
#include "IPrefab.h"

class SmallBoatPrefab : public IPrefab
{
public:
	SmallBoatPrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~SmallBoatPrefab();
};

