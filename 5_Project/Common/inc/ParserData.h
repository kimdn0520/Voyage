#pragma once
#include <algorithm>
#include "SimpleMath.h"

namespace NoobData
{ 
	struct Vertex
	{
		Vertex() 
		{
			for (int i = 0; i < 8; i++)
			{
				weights[i] = 0.f;
				boneIndices[i] = 0;
			}
		}
		Vertex(DirectX::SimpleMath::Vector3 paPos, 
			DirectX::SimpleMath::Vector4 paColor = { 1.0f, 1.0f, 1.0f, 1.0f }, 
			DirectX::SimpleMath::Vector2 paUV = { 0.f, 0.f }, 
			DirectX::SimpleMath::Vector3 paNormal = { 0.f, 0.f, 0.f }, 
			DirectX::SimpleMath::Vector3 paTangent = { 0.f, 0.f, 0.f })
			: pos(paPos), normal(paNormal), uv(paUV), tangent(paTangent), color(paColor)
		{
			for (int i = 0; i < 8; i++)
			{
				weights[i] = 0.f;
				boneIndices[i] = 0;
			}
		}

		DirectX::SimpleMath::Vector3 pos;
		DirectX::SimpleMath::Vector4 color;
		DirectX::SimpleMath::Vector2 uv;
		DirectX::SimpleMath::Vector3 normal;
		DirectX::SimpleMath::Vector3 tangent;

		// Skinned Data
		float weights[8];
		unsigned __int32 boneIndices[8];
	};

	struct FbxMaterialInfo
	{
		bool isDiffuse = false;
		bool isNormal = false;
		bool isSpecular = false;

		DirectX::SimpleMath::Vector4			diffuse;
		DirectX::SimpleMath::Vector4			ambient;
		DirectX::SimpleMath::Vector4			specular;

		std::string		name;
		std::wstring	diffuseTexName;
		std::wstring	normalTexName;
		std::wstring	specularTexName;
	};

	struct BoneWeight
	{
		using Pair = std::pair<int, double>;
		std::vector<Pair> boneWeights;

		void AddWeights(int index, double weight)
		{
			if (weight <= 0.f)
				return;

			auto findIt = std::find_if(boneWeights.begin(), boneWeights.end(),
				[=](const Pair& p) { return p.second < weight; });

			if (findIt != boneWeights.end())
				boneWeights.insert(findIt, Pair(index, weight));
			else
				boneWeights.push_back(Pair(index, weight));

			// 가중치는 최대 4개
			if (boneWeights.size() > 4)
				boneWeights.pop_back();
		}

		void Normalize()
		{
			double sum = 0.f;
			std::for_each(boneWeights.begin(), boneWeights.end(), [&](Pair& p) { sum += p.second; });
			std::for_each(boneWeights.begin(), boneWeights.end(), [=](Pair& p) { p.second = p.second / sum; });
		}
	};

	struct FbxMeshInfo
	{
		size_t							name;
		std::vector<Vertex>			vertices;

		std::vector<unsigned int>		indices;
		std::vector<FbxMaterialInfo>	materials;

		// 애니메이션 관련 부분
		std::vector<BoneWeight>			boneWeights; // 뼈 가중치

		bool isAnimation;	// 애니메이션 보유 여부
	};

	struct FbxKeyFrameInfo
	{
		DirectX::SimpleMath::Vector3 scale;
		DirectX::SimpleMath::Vector3 rotation;
		DirectX::SimpleMath::Vector3 translation;
		double		time;
	};

	struct FbxBoneInfo
	{
		std::string				boneName;
		int						parentIndex;
		//fbxsdk::FbxAMatrix				matOffset;
		DirectX::SimpleMath::Matrix matOffset;
	};


	struct FbxAnimClipInfo
	{
		std::string					name;
		float	ticksPerFrame;
		int		totalFrame;
		int		startFrame;
		int		endFrame;

		// keyFrames[boneIndex] = keyFrameInfo
		std::vector<std::vector<std::shared_ptr<NoobData::FbxKeyFrameInfo>>> keyFrames;
	};
}

namespace NoobData
{
	class FbxModel
	{
	public:
		std::vector<FbxMeshInfo> fbxMeshInfos;
		std::vector<std::shared_ptr<FbxBoneInfo>> fbxBoneInfo;
		std::vector<std::shared_ptr<FbxAnimClipInfo>> animationClipList;
	};
}

namespace NoobData
{
	struct ASEFrameData
	{
		std::string			fileName;
		int					firstFrame;
		int					lastFrame;
		int					frameSpeed;
		int					ticksPerFrame;
	};

	struct ASEMaterialInfo
	{
		bool isDiffuse = false;
		bool isNormal = false;
		bool isSpecular = false;

		DirectX::SimpleMath::Vector4			diffuse;
		DirectX::SimpleMath::Vector4			ambient;
		DirectX::SimpleMath::Vector4			specular;

		std::string		name;
		std::wstring	diffuseTexName;
		std::wstring	normalTexName;
		std::wstring	specularTexName;
	};

	struct Animation_pos
	{
		int			tick;
		DirectX::SimpleMath::Vector4		pos;
	};

	struct Animation_rot
	{
		int			tick;
		DirectX::SimpleMath::Vector4		rotQT;		// 상대변량이기 때문에 이전 값들과 누적해서 가지고 있는다 (즉 지금까지의 절대량으로 변환한다)
	};

	struct Animation_scl 
	{
		int			time;
		DirectX::SimpleMath::Vector3		scale;
		DirectX::SimpleMath::Vector3		scaleaxis;
		float		scaleaxisang;
		//Quaternion	m_scalerotQT_accumulation;
	};

	///--------------------------------------------------
	/// 위 클래스를 조합한 Animation 클래스
	///--------------------------------------------------
	class ASEAnimation
	{

	public:
		std::string		nodename;	// 그냥 오브젝트 이름
		std::size_t		animName;	// 애니메이션 이름
		std::vector<std::shared_ptr<Animation_pos>>		position;
		std::vector<std::shared_ptr<Animation_rot>>		rotation;
		std::vector<std::shared_ptr<Animation_scl>>		scale;

		//// 애니메이션 firstframe
		//int firstFrame;

		//// 애니메이션 총 프레임 수
		//int totalFrame;

		//// 애니메이션 초당 프레임 수
		//int frameSpeed;

		//// 프레임당 몇 Tick이 지나가는지
		//int ticksPerFrame;
	};

	struct ASEBoneInfo
	{
		std::string				boneName;
	};

	struct ASEMeshInfo
	{
		size_t							meshName;
		std::string						gameObjName;	// Node Name
		std::string						parentName;

		std::vector<Vertex>				vertices;	
		std::vector<unsigned int>		indices;

		unsigned int materialID;
		
		DirectX::SimpleMath::Matrix nodeTM;

		bool isAnimation;	// 애니메이션 보유 여부
		
		std::vector<std::shared_ptr<ASEAnimation>> animationClips;

		std::vector<std::shared_ptr<ASEBoneInfo>> boneInfos;
	};

}

namespace NoobData
{
	class ASEModel
	{
	public:
		std::shared_ptr<ASEFrameData> frameData;

		std::vector<std::shared_ptr<ASEMeshInfo>> meshInfos;

		std::vector<std::shared_ptr<ASEMaterialInfo>> materials;

		// box collider를 위한...것
		DirectX::SimpleMath::Vector3 minPos;
		DirectX::SimpleMath::Vector3 maxPos;
	};
}