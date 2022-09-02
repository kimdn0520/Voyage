#include "pch.h"
#include "MoneyData.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshData.h"
#include "Resources.h"
#include "Animator.h"
#include "MonoBehaviour.h"
#include "ObjectInfo.h"

MoneyData::MoneyData()
{}

MoneyData::~MoneyData()
{}

void MoneyData::LoadASEMeshData()
{
	_meshData = Resources::GetInstance()->LoadASE("Data/ASE/money.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID);
}

vector<shared_ptr<GameObject>> MoneyData::Instantiate()
{
	vector<shared_ptr<GameObject>> gameObjects = _meshData->Instantiate();

	shared_ptr<GameObject> sampleObj = make_shared<GameObject>();
	sampleObj->SetName("Money");					// 함수안쪽에서 size_t로 변환해서 저장된다.
	sampleObj->AddComponent<Transform>();
	sampleObj->GetComponent<Transform>()->SetLocalScale({ 1.f, 1.f, 1.f });
	sampleObj->GetComponent<Transform>()->SetLocalPosition({ 0.f, 0.f, 0.f });

	for (auto& gameObj : gameObjects)
	{
		if (gameObj->GetTransform()->GetParent().lock() == nullptr)
		{
			gameObj->GetTransform()->SetParent(sampleObj->GetTransform());
			sampleObj->SetChild(gameObj);				// 최상위 부모에게 자식으루..
		}
	}
	// 맨 앞에 최상위 오브젝트 삽입
	gameObjects.insert(gameObjects.begin(), sampleObj);

	return gameObjects;
}

void MoneyData::Release()
{
	// TODO : 
}
