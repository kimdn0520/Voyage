#include "pch.h"
#include "LilyFlower.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshData.h"
#include "Resources.h"
#include "Animator.h"
#include "MonoBehaviour.h"
#include "ObjectInfo.h"

LilyFlower::LilyFlower()
{}

LilyFlower::~LilyFlower()
{}

void LilyFlower::LoadASEMeshData()
{
	_meshData = Resources::GetInstance()->LoadASE("Data/ASE/lily.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID);
}

vector<shared_ptr<GameObject>> LilyFlower::Instantiate()
{
	vector<shared_ptr<GameObject>> gameObjects = _meshData->Instantiate();

	shared_ptr<GameObject> sampleObj = make_shared<GameObject>();
	sampleObj->SetName("lilyFlower");					// �Լ����ʿ��� size_t�� ��ȯ�ؼ� ����ȴ�.
	sampleObj->AddComponent<Transform>();
	sampleObj->GetComponent<Transform>()->SetLocalScale({ 1.f, 1.f, 1.f });
	sampleObj->GetComponent<Transform>()->SetLocalPosition({ 0.f, 0.f, 0.f });

	for (auto& gameObj : gameObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(sampleObj->GetTransform());
			sampleObj->SetChild(gameObj);				// �ֻ��� �θ𿡰� �ڽ�����..
		}
	}
	// �� �տ� �ֻ��� ������Ʈ ����
	gameObjects.insert(gameObjects.begin(), sampleObj);

	return gameObjects;
}

void LilyFlower::Release()
{
	// TODO : 
}
