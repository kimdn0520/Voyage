#include "pch.h"
#include "MeshRenderer.h"
#include "GraphicsEngineManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "SkinAnimator.h"
#include "GraphicsEngineAPI.h"

MeshRenderer::MeshRenderer(shared_ptr<GameObject> gameObject)
	: Renderer(gameObject),
	 _transform(gameObject->GetComponent<Transform>()),
	_meshInfo(make_shared<MeshInfo>())
{}

MeshRenderer::~MeshRenderer()
{}

void MeshRenderer::PushMaterial()
{
	shared_ptr<MaterialInfo> material = make_shared<MaterialInfo>();

	// ResourceManager에 있는 materials에 담을까? aseParser의 static 값 하나 증가시키고..
	// 해당하는 그 index를 가지고 있고...
	//_meshInfo->materials.push_back(material);
}


void MeshRenderer::Render()
{
	_meshInfo->worldTM = _transform->GetWorldMatrix();
	_meshInfo->worldPos = _transform->GetWorldPosition();

	if (_skinAnimator != nullptr)
	{
		_meshInfo->isSkinned = true;
		memcpy(&_meshInfo->finalBoneListMatrix, _skinAnimator->_finalBoneListMatrix, sizeof(Matrix) * 96);
	}

	if (!_meshInfo->isBone)
		GraphicsEngineManager::GetInstance()->SetObjInfo(_meshInfo);

	/*if (_meshInfo->type == OBJECT_TYPE::WATER)
	{
		SetCubeFaceCamera();
		GraphicsEngineManager::GetInstance()->SetCubeMapCamera();
	}*/
}
