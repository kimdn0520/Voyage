#pragma once
#include "IPrefab.h"

class HeartPrefab : public IPrefab
{
public:
	HeartPrefab(Vector3 pos = {0.f, 0.f, 0.f});
	~HeartPrefab();
};

