#pragma once
#include "Data.h"

class MoneyData : public Data
{
public:
	MoneyData();
	~MoneyData();

public:
	shared_ptr<MeshData> GetMeshData() { return _meshData; }

	void LoadASEMeshData() override;

	vector<shared_ptr<GameObject>> Instantiate() override;

	void Release() override;
};