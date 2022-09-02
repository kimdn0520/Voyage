#pragma once
#include "Data.h"

class LilyFlower : public Data
{
public:
	LilyFlower();
	~LilyFlower();

public:
	shared_ptr<MeshData> GetMeshData() { return _meshData; }

	void LoadASEMeshData() override;

	vector<shared_ptr<GameObject>> Instantiate() override;

	void Release() override;
};