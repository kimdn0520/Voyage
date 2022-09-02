#pragma once
#include "IPrefab.h"

class RosePrefab : public IPrefab
{
public:
	RosePrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~RosePrefab();
};

