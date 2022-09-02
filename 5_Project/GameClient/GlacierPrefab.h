#pragma once
#include "IPrefab.h"

class GlacierPrefab : public IPrefab
{
public:
	GlacierPrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~GlacierPrefab();
};

