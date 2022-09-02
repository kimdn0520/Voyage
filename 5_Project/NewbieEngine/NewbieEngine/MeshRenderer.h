#pragma once
#include "NewbieEngineDLL.h"
#include "Renderer.h"
#include "ObjectInfo.h"

class Transform;
class SkinAnimator;

class MeshRenderer : public Renderer
{
public:
	NewbieEngine_DLL MeshRenderer(shared_ptr<GameObject> gameObject);
	virtual ~MeshRenderer();

private:
	shared_ptr<Transform> _transform;

	shared_ptr<SkinAnimator> _skinAnimator;
	
	shared_ptr<MeshInfo> _meshInfo;

public:
	shared_ptr<MeshInfo> GetMeshInfo() { return _meshInfo; }

	void SetSkinAnimator(shared_ptr<SkinAnimator> skinAnimator) { _skinAnimator = skinAnimator;}

	NewbieEngine_DLL void SetMesh(size_t mesh) { _meshInfo->name = mesh; }

	NewbieEngine_DLL void PushMaterial();

	void SetIsBone(bool val) { _meshInfo->isBone = val; }

	void Render() override;
};
