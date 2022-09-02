#include "pch.h"
#include "MeshData.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Animator.h"
#include "SkinAnimator.h"
#include "MonoBehaviour.h"

MeshData::MeshData()
{}

MeshData::~MeshData()
{}

void MeshData::SetMeshRenderInfo(size_t name, string gameObjName, string parentName, unsigned int materialID, Matrix nodeTM, vector<shared_ptr<BoneInfo>> boneInfo)
{
	shared_ptr<RenderInfo> renderInfo = make_shared<RenderInfo>();
	renderInfo->name = name;
	renderInfo->gameObjName = gameObjName;
	renderInfo->parentName = parentName;
	renderInfo->materialID = materialID;
	renderInfo->bones = boneInfo;
	renderInfo->nodeTM = nodeTM;
	renderInfo->isAnimation = false;

	_renders.push_back(renderInfo);
}

void MeshData::SetMeshRenderInfo(size_t name, string gameObjName, string parentName, unsigned int materialID, Matrix nodeTM, vector<shared_ptr<AnimationClipInfo>> animClipInfos, vector<shared_ptr<BoneInfo>> boneInfo)
{
	shared_ptr<RenderInfo> renderInfo = make_shared<RenderInfo>();
	renderInfo->name = name;
	renderInfo->gameObjName = gameObjName;
	renderInfo->parentName = parentName;
	renderInfo->materialID = materialID;
	renderInfo->animClips = animClipInfos;
	renderInfo->bones = boneInfo;
	renderInfo->nodeTM = nodeTM;
	renderInfo->isAnimation = true;

	_renders.push_back(renderInfo);
}

vector<shared_ptr<GameObject>> MeshData::Instantiate()
{
	vector<shared_ptr<GameObject>> gameObjects;

	hash<string> hasher;

	for (auto& render : _renders)
	{
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->isCheckFrustum = true;			// �ø�üũ�Ұž�..
		gameObject->SetName(render->gameObjName);
		gameObject->AddComponent<Transform>();
		gameObject->GetComponent<Transform>()->SetNodeTM(render->nodeTM);
		gameObject->AddComponent<MeshRenderer>();
		gameObject->GetComponent<MeshRenderer>()->SetMesh(render->name);
		
		gameObject->GetComponent<MeshRenderer>()->GetMeshInfo()->materialID = render->materialID;

		// �ִϸ��̼��� �ִٸ�..!
		if (render->isAnimation)
		{
			gameObject->AddComponent<Animator>();

			for (int animCnt = 0; animCnt < render->animClips.size(); animCnt++)
			{
				gameObject->GetComponent<Animator>()->SetAnimClip(render->animClips[animCnt]->animName, render->animClips[animCnt]);
				gameObject->GetComponent<Animator>()->PushTickData(render->animClips[animCnt]->animName);
			}

			// ���� ù��°���� �������
			gameObject->GetComponent<Animator>()->Play(render->animClips[0]->animName, true);
		}

		if (!render->bones.empty())
		{
			gameObject->AddComponent<SkinAnimator>();
			gameObject->GetComponent<MeshRenderer>()->SetSkinAnimator(gameObject->GetComponent<SkinAnimator>());

			for (auto& boneNameList : render->bones)
			{
				gameObject->GetComponent<SkinAnimator>()->SetBoneNameList(boneNameList->boneName);
			}
		}

		Vector3 localScale = { 1.0f, 1.0f, 1.0f };
		Vector3 localRotation = { 0.f, 0.f, 0.f };
		Vector3 localTranslation = { 0.f, .0f, 0.f };
		if (render->parentName != "")
		{
			// ��.. �θ� ���� �������
			for (auto& gameObj : gameObjects)
			{
				if (gameObj->GetName() == hasher(render->parentName.c_str()))
				{
					gameObject->GetTransform()->SetParent(gameObj->GetTransform());
					gameObj->SetChild(gameObject);
					XMMATRIX parentMatrixInverse = XMMatrixInverse(nullptr, gameObj->GetComponent<Transform>()->GetWorldMatrix());
					XMMATRIX multipleMatrix = XMMatrixMultiply(gameObject->GetTransform()->GetNodeMatrix(), parentMatrixInverse);
					
					Transform::DecomposeMatrix(multipleMatrix, localScale, localRotation, localTranslation);
					break;
				}
			}
		}
		else
		{
			Transform::DecomposeMatrix(gameObject->GetTransform()->GetNodeMatrix(), localScale, localRotation, localTranslation);
		}

		gameObject->GetComponent<Transform>()->SetLocalScale(localScale);
		gameObject->GetComponent<Transform>()->SetLocalRotation(localRotation);
		gameObject->GetComponent<Transform>()->SetLocalPosition(localTranslation);
		gameObject->GetComponent<Transform>()->SetOriScale(localScale);
		gameObject->GetComponent<Transform>()->SetOriRot(localRotation);
		gameObject->GetComponent<Transform>()->SetOriTrans(localTranslation);

		gameObjects.push_back(gameObject);
		_gameObjects.insert(make_pair(gameObject->GetName(), gameObject));
		gameObject->GetComponent<Transform>()->FixedUpdate();
	}

	// ��Ų �ִϸ����͸� ���� ������Ʈ�� ã�´�.
	for (auto& gameObj : gameObjects)
	{
		shared_ptr<SkinAnimator> skinAnimator = gameObj->GetComponent<SkinAnimator>();

		if (skinAnimator != nullptr)
		{
			for (auto& skinNameList : skinAnimator->boneNameList)
			{
				_gameObjects[skinNameList]->GetComponent<MeshRenderer>()->SetIsBone(true);	// �ߺ��ɼ� �ִµ�.. ��¿����..?
				skinAnimator->boneObjList.push_back(_gameObjects[skinNameList]);
			}
		}
	}

	// �� Ŭ��� �����༭..!! ���� ������ �����Ҷ� ������ ����� �־���.. isBone�� ����� �ȹ�������..
	_gameObjects.clear();

    return gameObjects;
}