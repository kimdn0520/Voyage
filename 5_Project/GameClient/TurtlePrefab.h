#pragma once
#include "IPrefab.h"

class TurtlePrefab : public IPrefab
{
public:
	TurtlePrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~TurtlePrefab();
};

