#pragma once
#include "IPrefab.h"

class BigStarPrefab : public IPrefab
{
public:
	BigStarPrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~BigStarPrefab();
};

