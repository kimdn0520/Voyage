#pragma once
#include "IPrefab.h"

class ClockPrefab : public IPrefab
{
public:
	ClockPrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~ClockPrefab();
};

