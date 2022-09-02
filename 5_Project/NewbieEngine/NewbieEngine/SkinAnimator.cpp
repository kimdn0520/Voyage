#include "pch.h"
#include "SkinAnimator.h"
#include "GameObject.h"
#include "Transform.h"

SkinAnimator::SkinAnimator(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::SKINANIMATOR),
	_transform(gameObject->GetTransform())
{
	ZeroMemory(_finalBoneListMatrix, sizeof(XMMATRIX) * 96);
}

SkinAnimator::~SkinAnimator()
{
}


/// <summary>
/// �޽÷������� Render() �Լ��� ���ؼ� ���߿� �ѱ�⶧����
/// �޽÷����� ������ finalBoneListMatrix�� �Ѱ��ָ�ȴ�.
/// </summary>
void SkinAnimator::Update()
{
	Matrix boneWorldTM;
	Matrix boneNodeTM;
	Matrix boneNodeTM_Inverse;
	Matrix boneOffsetTM;
	Matrix boneOffsetTM_Inverse;
	Matrix skinWorldTM;
	Matrix skinWorldTM_Inverse;
	Matrix finalBoneTM;

	for (int i = 0; i < boneObjList.size(); i++)
	{
		shared_ptr<GameObject> tmpBone = boneObjList[i];

		boneWorldTM = tmpBone->GetTransform()->GetWorldMatrix();
		boneNodeTM = tmpBone->GetTransform()->GetNodeMatrix();

		Matrix skinWorldTM = _transform->GetWorldMatrix();
		Matrix skinNodeTM = _transform->GetNodeMatrix();
		skinWorldTM_Inverse = XMMatrixInverse(nullptr, skinWorldTM);

		boneNodeTM_Inverse = XMMatrixInverse(nullptr, boneNodeTM);

		// skinNode�� boneNode ������� ���ϸ� boneNode���� skinNode�� ����Ű�� ����� ������
		// boneWorld�� ���ؼ� world�� ������ ��Ű�׼��̴����� �ڽ���world�� �ι������ְ� �־����..
		// skinWorldTM_Inverse�� ����ٰ� �����ְų� ��Ű�׼��̴����� ���� �ϳ� ����.
		finalBoneTM = skinNodeTM * boneNodeTM_Inverse * boneWorldTM * skinWorldTM_Inverse;

		_finalBoneListMatrix[i] = finalBoneTM;
	}

	/*Matrix boneWorldTM;

	Matrix boneOffsetTM;

	Matrix boneNodeTM;

	Matrix finalBoneTM;

	for (int i = 0; i < boneObjList.size(); i++)
	{
		std::shared_ptr<Transform> boneTransform = boneObjList[i]->GetTransform();

		boneWorldTM = boneTransform->GetWorldMatrix();

		boneNodeTM = boneObjList[i]->GetTransform()->GetNodeMatrix();

		boneOffsetTM = boneNodeTM * _transform->GetNodeMatrix().Invert();

		_finalBoneListMatrix[i] = boneOffsetTM.Invert() * boneWorldTM * _transform->GetWorldMatrix().Invert();
	}*/
}
