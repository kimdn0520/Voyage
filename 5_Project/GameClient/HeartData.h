#pragma once
#include "Data.h"

class HeartData : public Data
{
public:
	HeartData();
	~HeartData();

public:
	shared_ptr<MeshData> GetMeshData() { return _meshData; }

	void LoadASEMeshData() override;

	vector<shared_ptr<GameObject>> Instantiate() override;

	void Release() override;
};