#pragma once
#include "NewbieEngineDLL.h"

class GameObject;
struct Material;
struct AnimationClipInfo;
struct BoneInfo;

struct RenderInfo
{
	size_t name;		// �׷��Ƚ� ���ҽ��Ŵ����� ����� �޽�name

	string gameObjName = "";

	string parentName = "";

	unsigned int materialID;
	
	int pass = 0;		// � �н��� �� ���ΰ�

	vector<shared_ptr<AnimationClipInfo>>	animClips;

	vector<shared_ptr<BoneInfo>>			bones;

	Matrix nodeTM;

	bool isAnimation;
};

class MeshData
{
public:
	NewbieEngine_DLL MeshData();
	NewbieEngine_DLL ~MeshData();

private:
	vector<shared_ptr<RenderInfo>> _renders;

	map<size_t, shared_ptr<GameObject>> _gameObjects;

	Vector3 _minPos;
	Vector3 _maxPos;

public:
	void SetMeshRenderInfo(size_t name, string gameObjName, string parentName, unsigned int materialID, Matrix nodeTM, vector<shared_ptr<BoneInfo>> boneInfo);
	void SetMeshRenderInfo(size_t name, string gameObjName, string parentName, unsigned int materialID, Matrix nodeTM, vector<shared_ptr<AnimationClipInfo>> animClipInfos, vector<shared_ptr<BoneInfo>> boneInfo);
	void SetMeshMinMaxPos(Vector3 minPos, Vector3 maxPos)
	{
		_minPos = minPos;
		_maxPos = maxPos;
	}

	const Vector3& GetMinPos() { return _minPos; }
	const Vector3& GetMaxPos() { return _maxPos; }

	NewbieEngine_DLL const int& GetGameObjectCnt() { return _gameObjects.size(); }

	// ���ӿ�����Ʈ�� ������ش�.
	NewbieEngine_DLL vector<shared_ptr<GameObject>> Instantiate();
};

