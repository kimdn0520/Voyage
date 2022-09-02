#pragma once
#include "IPrefab.h"

class StarPrefab : public IPrefab
{
public:
	StarPrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~StarPrefab();
};

