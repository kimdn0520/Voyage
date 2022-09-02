#pragma once
#include "IPrefab.h"

class IslandPrefab : public IPrefab
{
public:
	IslandPrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~IslandPrefab();
};

