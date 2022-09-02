#pragma once
#include "IPrefab.h"

class BottlePrefab : public IPrefab
{
public:
	BottlePrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~BottlePrefab();
};

