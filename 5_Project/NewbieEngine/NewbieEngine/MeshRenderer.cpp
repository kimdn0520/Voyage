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

	// ResourceManager�� �ִ� materials�� ������? aseParser�� static �� �ϳ� ������Ű��..
	// �ش��ϴ� �� index�� ������ �ְ�...
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
