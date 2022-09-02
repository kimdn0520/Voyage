#include "pch.h"
#include "FBXLoader.h"

FBXLoader::FBXLoader()
{

}

FBXLoader::~FBXLoader()
{
	if (_scene)
		_scene->Destroy();
	if (_manager)
		_manager->Destroy();
}

std::shared_ptr<NoobData::FbxModel> FBXLoader::LoadFbx(const std::wstring& path)
{
	// fbxModel 생성
	_fbxModel = std::make_shared<NoobData::FbxModel>();

	// 파일 데이터 로드
	Import(path);

	// Animation	
	LoadBones(_scene->GetRootNode());
	LoadAnimationInfo();

	// 로드된 데이터 파싱 (Mesh/Material/Skin)
	// TODO : 이게 겁나느려서 바꿔야함
	ParseNode(_scene->GetRootNode());

	Optimize();

	return _fbxModel;
}

void FBXLoader::Release()
{
	// TODO : ..
}

void FBXLoader::Import(const std::wstring& path)
{
	// FBX SDK 관리자 객체 생성
	_manager = FbxManager::Create();

	// IOSettings 객체 생성 및 설정
	FbxIOSettings* settings = FbxIOSettings::Create(_manager, IOSROOT);
	_manager->SetIOSettings(settings);

	// 씬 객체 생성
	_scene = FbxScene::Create(_manager, "");

	// 안쓸듯
	// _resourceDirectory = fs::path(path).parent_path().wstring() + L"/Texture/" /* + fs::path(path).filename().stem().wstring() + L".fbm"*/;

	// FbxImporter 객체 생성
	_importer = FbxImporter::Create(_manager, "");

	std::string strPath = ws2s(path);

	// importer 초기화
	_importer->Initialize(strPath.c_str(), -1, _manager->GetIOSettings());

	// fbx 파일 내용을 scene 으로 가져온다.
	_importer->Import(_scene);

	_scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::DirectX);

	// 씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다. 
	// 겁나오래걸리니까 무조건 삼각형으로 받아야할듯?
	FbxGeometryConverter geometryConverter(_manager);
	geometryConverter.Triangulate(_scene, true);

	_importer->Destroy();
}

void FBXLoader::ParseNode(FbxNode* node)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute)
	{
		switch (attribute->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			LoadMesh(node->GetMesh());
			break;
		}
	}

	// Material 로드
	const int materialCount = node->GetMaterialCount();
	for (int i = 0; i < materialCount; ++i)
	{
		FbxSurfaceMaterial* surfaceMaterial = node->GetMaterial(i);
		LoadMaterial(surfaceMaterial);
	}

	// Tree 구조 재귀 호출
	const int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; ++i)
		ParseNode(node->GetChild(i));
}

void FBXLoader::LoadMesh(FbxMesh* mesh)
{
	_fbxModel->fbxMeshInfos.push_back(NoobData::FbxMeshInfo());
	NoobData::FbxMeshInfo& meshInfo = _fbxModel->fbxMeshInfos.back();

	std::hash<std::string> hasher;

	meshInfo.name = hasher(mesh->GetName());

	const int vertexCount = mesh->GetControlPointsCount();
	//meshInfo.vertices.resize(vertexCount);
	meshInfo.vertices.resize(vertexCount);
	meshInfo.boneWeights.resize(vertexCount);

	// Position정보를 가져옴(축 바꿔서 가져옴)
	FbxVector4* controlPoints = mesh->GetControlPoints();
	for (int i = 0; i < vertexCount; ++i)
	{
		meshInfo.vertices[i].pos.x = static_cast<float>(controlPoints[i].mData[0]);
		meshInfo.vertices[i].pos.y = static_cast<float>(controlPoints[i].mData[2]);
		meshInfo.vertices[i].pos.z = static_cast<float>(controlPoints[i].mData[1]);
	}

	const int materialCount = mesh->GetNode()->GetMaterialCount();
	//meshInfo.indices.resize(materialCount);

	FbxGeometryElementMaterial* geometryElementMaterial = mesh->GetElementMaterial();

	const int polygonSize = mesh->GetPolygonSize(0);
	assert(polygonSize == 3);		// 삼각형이 아니면 오류

	int arrIdx[3];
	int vertexCounter = 0; // 정점의 개수

	const int triCount = mesh->GetPolygonCount(); // 메쉬의 삼각형 개수를 가져온다
	for (int i = 0; i < triCount; i++) // 삼각형의 개수
	{
		for (int j = 0; j < 3; j++) // 삼각형은 세 개의 정점으로 구성
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j); // 제어점의 인덱스 추출
			arrIdx[j] = controlPointIndex;

			// normal, tangent, uv 정보를 가져온다.
			GetNormal(mesh, &meshInfo, controlPointIndex, vertexCounter);
			// tangent 정보를 가지고있다면.. 그걸쓸것이다.
			if (mesh->GetElementTangentCount() != 0)
				GetTangent(mesh, &meshInfo, controlPointIndex, vertexCounter);
			GetUV(mesh, &meshInfo, controlPointIndex, mesh->GetTextureUVIndex(i, j));

			vertexCounter++;
		}

		//const int subsetIdx = geometryElementMaterial->GetIndexArray().GetAt(i);
		//meshInfo.indices[subsetIdx].push_back(arrIdx[0]);
		//meshInfo.indices[subsetIdx].push_back(arrIdx[2]);
		//meshInfo.indices[subsetIdx].push_back(arrIdx[1]);
		meshInfo.indices.push_back(arrIdx[0]);
		meshInfo.indices.push_back(arrIdx[2]);
		meshInfo.indices.push_back(arrIdx[1]);
	}

	// fbxModel에 tangent 정보가 없다면 계산해서 tangent 값을 넣어주자!
	if (mesh->GetElementTangentCount() == 0)
	{
		for (int i = 0; i < meshInfo.indices.size(); i += 3) // 삼각형의 개수
		{
			int i0 = meshInfo.indices[i];
			int i1 = meshInfo.indices[i + 1];
			int i2 = meshInfo.indices[i + 2];

			// e1 = p1 - p0, e2 = p2 - p0
			Vector3 e1 = meshInfo.vertices[i1].pos - meshInfo.vertices[i0].pos;
			Vector3 e2 = meshInfo.vertices[i2].pos - meshInfo.vertices[i0].pos;

			float x1 = meshInfo.vertices[i1].uv.x - meshInfo.vertices[i0].uv.x; // u1 - u0
			float y1 = meshInfo.vertices[i1].uv.y - meshInfo.vertices[i0].uv.y; // v1 - v0
			float x2 = meshInfo.vertices[i2].uv.y - meshInfo.vertices[i0].uv.x; // u2 - u0
			float y2 = meshInfo.vertices[i2].uv.y - meshInfo.vertices[i0].uv.y; // v2 - v0

			float det = (x1 * y2) - (x2 * y1); // ad - bc
			if (det == 0.f) { det = 0.00001f; }
			float r = 1.0f / det;

			Vector3 tangent = (e1 * y2 - e2 * y1) * r;

			meshInfo.vertices[i0].tangent += tangent;
			meshInfo.vertices[i1].tangent += tangent;
			meshInfo.vertices[i2].tangent += tangent;
		}
	}

	// Animation 데이터를 받아준다.
	LoadAnimationData(mesh, &meshInfo);
}

void FBXLoader::LoadMaterial(FbxSurfaceMaterial* surfaceMaterial)
{
	NoobData::FbxMaterialInfo material{};

	material.name = surfaceMaterial->GetName();

	material.diffuse = GetMaterialData(surfaceMaterial, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	material.ambient = GetMaterialData(surfaceMaterial, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	material.specular = GetMaterialData(surfaceMaterial, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);

	material.diffuseTexName = GetTextureRelativeName(surfaceMaterial, FbxSurfaceMaterial::sDiffuse);
	material.normalTexName = GetTextureRelativeName(surfaceMaterial, FbxSurfaceMaterial::sNormalMap);
	material.specularTexName = GetTextureRelativeName(surfaceMaterial, FbxSurfaceMaterial::sSpecular);

	if (material.diffuseTexName != L"") { material.isDiffuse = true; }
	if (material.normalTexName != L"") { material.isNormal = true; }
	if (material.specularTexName != L"") { material.isSpecular = true; }

	_fbxModel->fbxMeshInfos.back().materials.push_back(material);
}

void FBXLoader::GetNormal(FbxMesh* mesh, NoobData::FbxMeshInfo* container, int idx, int vertexCounter)
{
	if (mesh->GetElementNormalCount() == 0)
		return;

	FbxGeometryElementNormal* normal = mesh->GetElementNormal();
	int normalIdx = 0;

	// vertex가 그것이 구성하는 polygon 만큼의 매핑 좌표를 가지게 될것이다.
	if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIdx = vertexCounter;
		else
			normalIdx = normal->GetIndexArray().GetAt(vertexCounter);
	}
	// 각 surface control point/vertex를 위해 하나의 매핑 좌표만 존재할 것이다.
	else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
			normalIdx = idx;
		else
			normalIdx = normal->GetIndexArray().GetAt(idx);
	}

	FbxVector4 vec = normal->GetDirectArray().GetAt(normalIdx);
	container->vertices[idx].normal.x = static_cast<float>(vec.mData[0]);
	container->vertices[idx].normal.y = static_cast<float>(vec.mData[2]);
	container->vertices[idx].normal.z = static_cast<float>(vec.mData[1]);
}

void FBXLoader::GetTangent(FbxMesh* mesh, NoobData::FbxMeshInfo* meshInfo, int idx, int vertexCounter)
{
	FbxGeometryElementTangent* tangent = mesh->GetElementTangent();
	int tangentIdx = 0;

	if (tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			tangentIdx = vertexCounter;
		else
			tangentIdx = tangent->GetIndexArray().GetAt(vertexCounter);
	}
	else if (tangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (tangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			tangentIdx = idx;
		else
			tangentIdx = tangent->GetIndexArray().GetAt(idx);
	}

	FbxVector4 vec = tangent->GetDirectArray().GetAt(tangentIdx);
	meshInfo->vertices[idx].tangent.x = static_cast<float>(vec.mData[0]);
	meshInfo->vertices[idx].tangent.y = static_cast<float>(vec.mData[2]);
	meshInfo->vertices[idx].tangent.z = static_cast<float>(vec.mData[1]);
}

void FBXLoader::GetUV(FbxMesh* mesh, NoobData::FbxMeshInfo* meshInfo, int idx, int uvIndex)
{
	FbxVector2 uv = mesh->GetElementUV()->GetDirectArray().GetAt(uvIndex);
	meshInfo->vertices[idx].uv.x = static_cast<float>(uv.mData[0]);
	meshInfo->vertices[idx].uv.y = static_cast<float>(uv.mData[1]);
}

Vector4 FBXLoader::GetMaterialData(FbxSurfaceMaterial* surface, const char* materialName, const char* factorName)
{
	FbxDouble3  material;
	FbxDouble	factor = 0.f;

	FbxProperty materialProperty = surface->FindProperty(materialName);
	FbxProperty factorProperty = surface->FindProperty(factorName);

	if (materialProperty.IsValid() && factorProperty.IsValid())
	{
		material = materialProperty.Get<FbxDouble3>();
		factor = factorProperty.Get<FbxDouble>();
	}

	Vector4 ret = Vector4(
		static_cast<float>(material.mData[0] * factor),
		static_cast<float>(material.mData[1] * factor),
		static_cast<float>(material.mData[2] * factor),
		static_cast<float>(factor));

	return ret;
}

std::wstring FBXLoader::GetTextureRelativeName(FbxSurfaceMaterial* surface, const char* materialProperty)
{
	std::string name;

	FbxProperty textureProperty = surface->FindProperty(materialProperty);
	if (textureProperty.IsValid())
	{
		int count = textureProperty.GetSrcObjectCount();

		if (1 <= count)
		{
			FbxFileTexture* texture = textureProperty.GetSrcObject<FbxFileTexture>(0);
			if (texture)
				name = texture->GetRelativeFileName();
		}
	}

	std::wstring relativePath = s2ws(name);
	std::wstring filename = fs::path(relativePath).filename();
	std::wstring realName = filename.substr(0, filename.size() - 4);

	return realName;
}

void FBXLoader::LoadBones(FbxNode* node, int idx, int parentIdx)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		std::shared_ptr<NoobData::FbxBoneInfo> bone = std::make_shared<NoobData::FbxBoneInfo>();
		bone->boneName = node->GetName();
		bone->parentIndex = parentIdx;
		_fbxModel->fbxBoneInfo.push_back(bone);
	}

	const int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; i++)
		LoadBones(node->GetChild(i), static_cast<int>(_fbxModel->fbxBoneInfo.size()), idx);
}

void FBXLoader::LoadAnimationInfo()
{
	_scene->FillAnimStackNameArray(OUT _animNames);

	const int animCount = _animNames.GetCount();		// 대충.. 애니메이션클립 갯수?

	// for문돌면서 애니메이션클립 마다 시간 넣어주는듯함
	for (int i = 0; i < animCount; i++)
	{
		FbxAnimStack* animStack = _scene->FindMember<FbxAnimStack>(_animNames[i]->Buffer());
		if (animStack == nullptr)
			continue;

		std::shared_ptr<NoobData::FbxAnimClipInfo> animClip = std::make_shared<NoobData::FbxAnimClipInfo>();
		animClip->name = animStack->GetName();
		animClip->keyFrames.resize(_fbxModel->fbxBoneInfo.size());			// 키프레임은 본의 개수만큼

		FbxTakeInfo* takeInfo = _scene->GetTakeInfo(animStack->GetName());
		float frameRate = (float)FbxTime::GetFrameRate(_scene->GetGlobalSettings().GetTimeMode());
		double tempStart = takeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
		double tempStop = takeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
		int keyFrames;			// Animation Total Frame
		int startTime;			// Animation Start Frame
		float tickFrame;		// One Frame Time

		if (tempStart < tempStop)
		{
			keyFrames = (int)((tempStop - tempStart) * (double)frameRate);
			tickFrame = (float)(tempStop - tempStart) / (float)keyFrames;
			startTime = (int)(tempStart)*keyFrames;

			// 한 프레임 재생 시간
			animClip->ticksPerFrame = tickFrame;

			// 시작 프레임
			animClip->startFrame = startTime;

			// 끝 프레임
			animClip->endFrame = keyFrames - 1;

			// 총 프레임
			animClip->totalFrame = keyFrames;
		}

		_fbxModel->animationClipList.push_back(animClip);
	}
}

void FBXLoader::LoadAnimationData(FbxMesh* mesh, NoobData::FbxMeshInfo* meshInfo)
{
	const int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);

	// 애니메이션이 없다면..
	if (skinCount <= 0 || _fbxModel->animationClipList.empty())
	{
		meshInfo->isAnimation = false;
		return;
	}
	meshInfo->isAnimation = true;

	for (int i = 0; i < skinCount; i++)
	{
		FbxSkin* fbxSkin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

		if (fbxSkin)
		{
			FbxSkin::EType type = fbxSkin->GetSkinningType();
			if (FbxSkin::eRigid == type || FbxSkin::eLinear)
			{
				const int clusterCount = fbxSkin->GetClusterCount();
				for (int j = 0; j < clusterCount; j++)
				{
					FbxCluster* cluster = fbxSkin->GetCluster(j);
					if (cluster->GetLink() == nullptr)
						continue;

					int boneIdx = FindBoneIndex(cluster->GetLink()->GetName());
					assert(boneIdx >= 0);

					FbxAMatrix matNodeTransform = GetTransform(mesh->GetNode());
					LoadBoneWeight(cluster, boneIdx, meshInfo);
					LoadOffsetMatrix(cluster, matNodeTransform, boneIdx, meshInfo);

					const int animCount = _animNames.Size();
					for (int k = 0; k < animCount; k++)
						LoadKeyframe(k, mesh->GetNode(), cluster, matNodeTransform, boneIdx, meshInfo);
				}
			}
		}
	}

	FillBoneWeight(mesh, meshInfo);
}


void FBXLoader::FillBoneWeight(FbxMesh* mesh, NoobData::FbxMeshInfo* meshInfo)
{
	const int size = static_cast<int>(meshInfo->boneWeights.size());
	for (int v = 0; v < size; v++)
	{
		NoobData::BoneWeight& boneWeight = meshInfo->boneWeights[v];
		boneWeight.Normalize();

		float animBoneIndex[4] = {};
		float animBoneWeight[4] = {};

		const int weightCount = static_cast<int>(boneWeight.boneWeights.size());
		for (int w = 0; w < weightCount; w++)
		{
			animBoneIndex[w] = static_cast<float>(boneWeight.boneWeights[w].first);
			animBoneWeight[w] = static_cast<float>(boneWeight.boneWeights[w].second);
		}

		/*memcpy(&meshInfo->vertices[v].weightsIndices, animBoneIndex, sizeof(Vector4));
		memcpy(&meshInfo->vertices[v].weights, animBoneWeight, sizeof(Vector4));*/
	}
}


void FBXLoader::LoadBoneWeight(FbxCluster* cluster, int boneIdx, NoobData::FbxMeshInfo* meshInfo)
{
	const int indicesCount = cluster->GetControlPointIndicesCount();
	for (int i = 0; i < indicesCount; i++)
	{
		double weight = cluster->GetControlPointWeights()[i];
		int vtxIdx = cluster->GetControlPointIndices()[i];
		meshInfo->boneWeights[vtxIdx].AddWeights(boneIdx, weight);
	}
}

void FBXLoader::LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int boneIdx, NoobData::FbxMeshInfo* meshInfo)
{
	FbxAMatrix matClusterTrans;
	FbxAMatrix matClusterLinkTrans;
	// The transformation of the mesh at binding time 
	cluster->GetTransformMatrix(matClusterTrans);
	// The transformation of the cluster(joint) at binding time from joint space to world space 
	cluster->GetTransformLinkMatrix(matClusterLinkTrans);

	FbxVector4 V0 = { 1, 0, 0, 0 };
	FbxVector4 V1 = { 0, 0, 1, 0 };
	FbxVector4 V2 = { 0, 1, 0, 0 };
	FbxVector4 V3 = { 0, 0, 0, 1 };

	FbxAMatrix matReflect;
	matReflect[0] = V0;
	matReflect[1] = V1;
	matReflect[2] = V2;
	matReflect[3] = V3;

	FbxAMatrix matOffset;
	matOffset = matClusterLinkTrans.Inverse() * matClusterTrans;
	matOffset = matReflect * matOffset * matReflect;

	_fbxModel->fbxBoneInfo[boneIdx]->matOffset = ConvertMatrix(matOffset.Transpose());
}

void FBXLoader::LoadKeyframe(int animIndex, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int boneIdx, NoobData::FbxMeshInfo* meshInfo)
{
	if (_fbxModel->animationClipList.empty())
		return;

	FbxVector4	v1 = { 1, 0, 0, 0 };
	FbxVector4	v2 = { 0, 0, 1, 0 };
	FbxVector4	v3 = { 0, 1, 0, 0 };
	FbxVector4	v4 = { 0, 0, 0, 1 };
	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	FbxTime::EMode timeMode = _scene->GetGlobalSettings().GetTimeMode();

	// 애니메이션 골라줌
	FbxAnimStack* animStack = _scene->FindMember<FbxAnimStack>(_animNames[animIndex]->Buffer());
	_scene->SetCurrentAnimationStack(OUT animStack);

	/*FbxLongLong startFrame = _fbxModel->animationClipList[animIndex]->startTime.GetFrameCount(timeMode);
	FbxLongLong endFrame = _fbxModel->animationClipList[animIndex]->endTime.GetFrameCount(timeMode);*/


	// 애니클립들 중 하나라 가져와서 keyFrames를 설정해주는듯함
	for (FbxLongLong frame = _fbxModel->animationClipList[animIndex]->startFrame; frame < _fbxModel->animationClipList[animIndex]->endFrame; frame++)
	{
		std::shared_ptr<NoobData::FbxKeyFrameInfo> keyFrameInfo = std::make_shared<NoobData::FbxKeyFrameInfo>();
		FbxTime fbxTime = 0;

		fbxTime.SetFrame(frame, timeMode);

		FbxAMatrix matFromNode = node->EvaluateGlobalTransform(fbxTime);
		FbxAMatrix matTransform = matFromNode.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(fbxTime);
		matTransform = matReflect * matTransform * matReflect;

		keyFrameInfo->time = fbxTime.GetSecondDouble();
		//keyFrameInfo->matTransform = ConvertMatrix(matTransform);
		keyFrameInfo->scale.x = matTransform.GetS().mData[0];
		keyFrameInfo->scale.y = matTransform.GetS().mData[1];
		keyFrameInfo->scale.z = matTransform.GetS().mData[2];
		keyFrameInfo->rotation.x = matTransform.GetR().mData[0];
		keyFrameInfo->rotation.y = matTransform.GetR().mData[1];
		keyFrameInfo->rotation.z = matTransform.GetR().mData[2];
		keyFrameInfo->translation.x = matTransform.GetT().mData[0];
		keyFrameInfo->translation.y = matTransform.GetT().mData[1];
		keyFrameInfo->translation.z = matTransform.GetT().mData[2];

		_fbxModel->animationClipList[animIndex]->keyFrames[boneIdx].push_back(keyFrameInfo);
		//_oneAnimation->keyFrames[boneIdx].push_back(keyFrameInfo);			
		//_fbxModel->animationList.push_back(_oneAnimation);
	}
}

void FBXLoader::Optimize()
{
	//for (int meshCnt = 0; meshCnt < _fbxModel->fbxMeshInfos.size(); meshCnt++)
	//{
	//	int ori_vertexCnt = _fbxModel->fbxMeshInfos[meshCnt].vertices.size();
	//	bool isNewVertex = false;
	//	int resize_vertex = _fbxModel->fbxMeshInfos[meshCnt].vertices.size();

	//	for (int i = 0; i < _fbxModel->fbxMeshInfos[meshCnt].indices.size(); i++)
	//	{
	//		int vertexIndex = _fbxModel->fbxMeshInfos[meshCnt].indices[i];

	//		//NoobData::FbxVertex* nowVerex = _fbxModel->fbxMeshInfos[meshCnt].vertices[vertexIndex];
	//		Vector3 nowNormal = _fbxModel->fbxMeshInfos[meshCnt].vertices[vertexIndex].normal;
	//		Vector2 nowUV = _fbxModel->fbxMeshInfos[meshCnt].vertices[vertexIndex].uv;
	//		// TODO : 버텍스 쪼개기 하쟈
	//	}
	//}
}
int FBXLoader::FindBoneIndex(std::string name)
{
	//std::wstring boneName = std::wstring(name.begin(), name.end());

	for (int i = 0; i < _fbxModel->fbxBoneInfo.size(); ++i)
	{
		if (_fbxModel->fbxBoneInfo[i]->boneName == name)
			return i;
	}

	return -1;
}

FbxAMatrix FBXLoader::GetTransform(FbxNode* node)
{
	const FbxVector4 translation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 rotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 scaling = node->GetGeometricScaling(FbxNode::eSourcePivot);
	return FbxAMatrix(translation, rotation, scaling);
}

Vector2 FBXLoader::ConvertVector2(fbxsdk::FbxVector2 v2)
{
	return Vector2(
		static_cast<float>(v2.mData[0]),
		1.0f - static_cast<float>(v2.mData[1]));
}

Vector2 FBXLoader::ConvertVector2(fbxsdk::FbxVector4 v4)
{
	return Vector2(
		static_cast<float>(v4.mData[0]),
		1.0f - static_cast<float>(v4.mData[1]));
}

Vector3 FBXLoader::ConvertVector3(fbxsdk::FbxVector4 v4)
{
	return Vector3(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[1]));
}

Vector4 FBXLoader::ConvertVector4(fbxsdk::FbxVector4 v4)
{
	return Vector4(
		static_cast<float>(v4.mData[0]),
		static_cast<float>(v4.mData[2]),
		static_cast<float>(v4.mData[1]),
		static_cast<float>(v4.mData[3]));
}

Matrix FBXLoader::ConvertMatrix(fbxsdk::FbxMatrix matrix)
{
	FbxVector4 r1 = matrix.GetRow(0);
	FbxVector4 r2 = matrix.GetRow(1);
	FbxVector4 r3 = matrix.GetRow(2);
	FbxVector4 r4 = matrix.GetRow(3);

	return Matrix
	(
		ConvertVector4(r1),
		ConvertVector4(r3),
		ConvertVector4(r2),
		ConvertVector4(r4)
	);
}