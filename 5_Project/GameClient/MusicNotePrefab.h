#pragma once
#include "IPrefab.h"

class MusicNotePrefab : public IPrefab
{
public:
	MusicNotePrefab(Vector3 pos = { 0.f, 0.f, 0.f });
	~MusicNotePrefab();
};

