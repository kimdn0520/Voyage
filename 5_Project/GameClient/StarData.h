#pragma once
#include "Data.h"

class StarData : public Data
{
public:
	StarData();
	~StarData();

public:
	shared_ptr<MeshData> GetMeshData() { return _meshData; }

	void LoadASEMeshData() override;

	vector<shared_ptr<GameObject>> Instantiate() override;

	void Release() override;
};