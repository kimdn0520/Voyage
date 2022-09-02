#pragma once
#include "SimpleMath.h"

struct CubeVertex
{
	CubeVertex() {}
	CubeVertex(DirectX::SimpleMath::Vector3 paPos, DirectX::SimpleMath::Vector4 paColor, DirectX::SimpleMath::Vector2 paUV, DirectX::SimpleMath::Vector3 paNoraml)
		: pos(paPos), color(paColor), uv(paUV), noraml(paNoraml) {}

	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector4 color;
	DirectX::SimpleMath::Vector2 uv;
	DirectX::SimpleMath::Vector3 noraml;
};

/// <summary>
/// 요녀석을 쓸거임..
/// </summary>
struct GraphicsVertex
{
	GraphicsVertex()
	{
		for (int i = 0; i < 8; i++)
		{
			weights[i] = 0.f;
			boneIndices[i] = 0;
		}
	}
	GraphicsVertex(DirectX::SimpleMath::Vector3 paPos,
		DirectX::SimpleMath::Vector4 paColor = { 1.0f, 1.0f, 1.0f, 1.0f },
		DirectX::SimpleMath::Vector2 paUV = { 0.f, 0.f },
		DirectX::SimpleMath::Vector3 paNormal = { 0.f, 0.f, 0.f },
		DirectX::SimpleMath::Vector3 paTangent = { 0.f, 0.f, 0.f })
	{
		pos = paPos; color = paColor; uv = paUV; normal = paNormal; tangent = paTangent;

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