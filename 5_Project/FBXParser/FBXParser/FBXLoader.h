#pragma once
#include "ParserBase.h"
#include "SimpleMath.h"
using namespace DirectX;
using namespace SimpleMath;

class FBXLoader : public ParserBase
{
public:
	FBXLoader();
	virtual~FBXLoader();

public:
    std::shared_ptr<NoobData::FbxModel> LoadFbx(const std::wstring& path) override;

	void Release() override;

public:
	Vector2 ConvertVector2(fbxsdk::FbxVector2 v2);
	Vector2 ConvertVector2(fbxsdk::FbxVector4 v4);
	Vector3 ConvertVector3(fbxsdk::FbxVector4 v4);
	Vector4 ConvertVector4(fbxsdk::FbxVector4 v4);
	Matrix ConvertMatrix(fbxsdk::FbxMatrix matrix);

public:
	int GetMeshCount() { return static_cast<int>(_fbxModel->fbxMeshInfos.size()); }
	const NoobData::FbxMeshInfo& GetMesh(int idx) { return _fbxModel->fbxMeshInfos[idx]; }
	std::vector<std::shared_ptr<NoobData::FbxBoneInfo>>& GetBones() { return _fbxModel->fbxBoneInfo; }
	std::vector<std::shared_ptr<NoobData::FbxAnimClipInfo>>& GetAnimClip() { return _fbxModel->animationClipList; }
private:
	void Import(const std::wstring& path);

	void ParseNode(fbxsdk::FbxNode* root);
	void LoadMesh(fbxsdk::FbxMesh* mesh);
	void LoadMaterial(FbxSurfaceMaterial* surfaceMaterial);

	void		GetNormal(fbxsdk::FbxMesh* mesh, NoobData::FbxMeshInfo* container, int idx, int vertexCounter);
	void		GetTangent(fbxsdk::FbxMesh* mesh, NoobData::FbxMeshInfo* container, int idx, int vertexCounter);
	void		GetUV(fbxsdk::FbxMesh* mesh, NoobData::FbxMeshInfo* container, int idx, int vertexCounter);
	Vector4		GetMaterialData(fbxsdk::FbxSurfaceMaterial* surface, const char* materialName, const char* factorName);
	std::wstring		GetTextureRelativeName(fbxsdk::FbxSurfaceMaterial* surface, const char* materialProperty);


	// Animation
	void LoadBones(fbxsdk::FbxNode* node) { LoadBones(node, 0, -1); }
	void LoadBones(fbxsdk::FbxNode* node, int idx, int parentIdx);
	void LoadAnimationInfo();

	void LoadAnimationData(fbxsdk::FbxMesh* mesh, NoobData::FbxMeshInfo* meshInfo);
	void LoadBoneWeight(fbxsdk::FbxCluster* cluster, int boneIdx, NoobData::FbxMeshInfo* meshInfo);
	void LoadOffsetMatrix(fbxsdk::FbxCluster* cluster, const fbxsdk::FbxAMatrix& matNodeTransform, int boneIdx, NoobData::FbxMeshInfo* meshInfo);
	void LoadKeyframe(int animIndex, fbxsdk::FbxNode* node, fbxsdk::FbxCluster* cluster, const fbxsdk::FbxAMatrix& matNodeTransform, int boneIdx, NoobData::FbxMeshInfo* container);

	int FindBoneIndex(std::string name);
	FbxAMatrix GetTransform(fbxsdk::FbxNode* node);

	void FillBoneWeight(fbxsdk::FbxMesh* mesh, NoobData::FbxMeshInfo* meshInfo);

	void Optimize();
private:
	fbxsdk::FbxManager* _manager = nullptr;
	fbxsdk::FbxScene* _scene = nullptr;
	fbxsdk::FbxImporter* _importer = nullptr;
	std::wstring			_resourceDirectory;

	fbxsdk::FbxArray<fbxsdk::FbxString*>					_animNames;

	// LoadFbx를 하면 이 fbxModel를 return 시켜 줄 것이다. 
	std::shared_ptr<NoobData::FbxModel>						_fbxModel;
	/*std::vector<NoobData::FbxMeshInfo>					_meshes;
	std::vector<std::shared_ptr<NoobData::FbxBoneInfo>>		_bones;
	std::vector<std::shared_ptr<NoobData::FbxAnimClipInfo>>	_animClips;*/
};