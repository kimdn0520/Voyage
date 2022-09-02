#include "pch.h"
#include "GraphicsEngineAPI.h"
#include "Resources.h"
#include "MeshData.h"
#include "ParserData.h"
#include "ParserBase.h"
#include "ASEParser.h"
#include "GraphicsEngineManager.h"
#include "ObjectInfo.h"
#include "ParticleInfo.h"

shared_ptr<Resources> Resources::resources = nullptr;

shared_ptr<Resources> Resources::GetInstance()
{
	if (resources == nullptr)
		resources = make_shared<Resources>();

	return resources;
}

void Resources::Init()
{
	// ase �� �غ�..
	_aseParser = make_shared<ASEParser>();
	
	string fmodPath = "Data/FmodBank/";

	InputEntryDir(fmodPath);

	LoadCubeMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID);

	LoadWireGreenCubeMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, (int)RasterState::WIRE);

	LoadWireRedCubeMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, (int)RasterState::WIRE);

	LoadSphereMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID);

	LoadWireSphereGreenMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, (int)RasterState::WIRE);

	LoadWireSphereRedMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, (int)RasterState::WIRE);

	LoadAxisMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, (int)RasterState::WIRE);

	LoadGridMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, (int)RasterState::WIRE);

	LoadScreenMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID);

	LoadRectMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID);

	LoadRectWireMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST, (int)RasterState::WIRE);

	LoadCircleMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID);

	LoadOceanMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID);

	LoadWaveMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID);
}

void Resources::CreateMeshBoundingCube(const std::vector<GraphicsVertex>& vertexVector, const size_t& meshName)
{
	float minX = 5000000.f;
	float maxX = -5000000.f;
	float minY = 5000000.f;
	float maxY = -5000000.f;
	float minZ = 5000000.f;
	float maxZ = -5000000.f;

	for (auto& vertex : vertexVector)
	{
		Vector3 pos = vertex.pos;

		float x = pos.x;
		float y = pos.y;
		float z = pos.z;

		maxX = std::max<float>(maxX, x);

		minX = std::min<float>(minX, x);

		maxY = std::max<float>(maxY, y);

		minY = std::min<float>(minY, y);

		maxZ = std::max<float>(maxZ, z);

		minZ = std::min<float>(minZ, z);
	}

	Vector3 localCenter = Vector3((maxX + minX) / 2, (maxY + minY) / 2, (maxZ + minZ) / 2);

	Vector3 localSide = Vector3(std::abs(maxX - minX), std::abs(maxY - minY), std::abs(maxZ - minZ));

	_meshBoundingCube.insert({ meshName, std::make_shared<BoundingCube>(localSide, localCenter) });
}

void Resources::LoadCubeMesh(int topology, int rasterState)
{
	float x = 0.5f;
	float y = 0.5f;
	float z = 0.5f;

	vector<GraphicsVertex> vertices(24);

	// �ո�
	vertices[0] = GraphicsVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[1] = GraphicsVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[2] = GraphicsVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	vertices[3] = GraphicsVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
	// �޸�
	vertices[4] = GraphicsVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[5] = GraphicsVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[6] = GraphicsVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	vertices[7] = GraphicsVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
	// ����
	vertices[8] = GraphicsVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[9] = GraphicsVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[10] = GraphicsVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
	vertices[11] = GraphicsVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
	// �Ʒ���
	vertices[12] = GraphicsVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[13] = GraphicsVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[14] = GraphicsVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	vertices[15] = GraphicsVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	// ���ʸ�		
	vertices[16] = GraphicsVertex(Vector3(-x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[17] = GraphicsVertex(Vector3(-x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[18] = GraphicsVertex(Vector3(-x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	vertices[19] = GraphicsVertex(Vector3(-x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f));
	// �����ʸ�		
	vertices[20] = GraphicsVertex(Vector3(+x, -y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[21] = GraphicsVertex(Vector3(+x, +y, -z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[22] = GraphicsVertex(Vector3(+x, +y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f));
	vertices[23] = GraphicsVertex(Vector3(+x, -y, +z), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));

	vector<unsigned int> idx(36);

	// �ո�
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// �޸�
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// ����
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// �Ʒ���
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ���ʸ�
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// �����ʸ�
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::CubeMesh,
		topology,
		rasterState, false);
}

void Resources::LoadWireGreenCubeMesh(int topology, int rasterState)
{
	float x = 0.5f;
	float y = 0.5f;
	float z = 0.5f;

	vector<GraphicsVertex> vertices(24);

	// �ո�
	vertices[0] = GraphicsVertex(Vector3(-x, +y, -z), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	vertices[1] = GraphicsVertex(Vector3(+x, +y, -z), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	vertices[2] = GraphicsVertex(Vector3(+x, -y, -z), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	vertices[3] = GraphicsVertex(Vector3(-x, -y, -z), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	vertices[4] = GraphicsVertex(Vector3(-x, +y, +z), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	vertices[5] = GraphicsVertex(Vector3(+x, +y, +z), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	vertices[6] = GraphicsVertex(Vector3(+x, -y, +z), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	vertices[7] = GraphicsVertex(Vector3(-x, -y, +z), Vector4(0.0f, 1.0f, 0.0f, 1.0f));

	vector<unsigned int> idx(24);

	idx[0] = 0; idx[1] = 1;
	idx[2] = 1; idx[3] = 2;
	idx[4] = 2; idx[5] = 3;
	idx[6] = 3; idx[7] = 0;
	idx[8] = 0; idx[9] = 4;
	idx[10] = 1; idx[11] = 5;
	idx[12] = 2; idx[13] = 6;
	idx[14] = 3; idx[15] = 7;
	idx[16] = 4; idx[17] = 5;
	idx[18] = 5; idx[19] = 6;
	idx[20] = 6; idx[21] = 7;
	idx[22] = 7; idx[23] = 4;

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::WireGreenCubeMesh,
		topology,
		rasterState, false);
}

void Resources::LoadWireRedCubeMesh(int topology, int rasterState)
{
	float x = 0.5f;
	float y = 0.5f;
	float z = 0.5f;

	vector<GraphicsVertex> vertices(24);

	// �ո�
	vertices[0] = GraphicsVertex(Vector3(-x, +y, -z), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[1] = GraphicsVertex(Vector3(+x, +y, -z), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[2] = GraphicsVertex(Vector3(+x, -y, -z), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[3] = GraphicsVertex(Vector3(-x, -y, -z), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[4] = GraphicsVertex(Vector3(-x, +y, +z), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[5] = GraphicsVertex(Vector3(+x, +y, +z), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[6] = GraphicsVertex(Vector3(+x, -y, +z), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[7] = GraphicsVertex(Vector3(-x, -y, +z), Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	vector<unsigned int> idx(24);

	idx[0] = 0; idx[1] = 1;
	idx[2] = 1; idx[3] = 2;
	idx[4] = 2; idx[5] = 3;
	idx[6] = 3; idx[7] = 0;
	idx[8] = 0; idx[9] = 4;
	idx[10] = 1; idx[11] = 5;
	idx[12] = 2; idx[13] = 6;
	idx[14] = 3; idx[15] = 7;
	idx[16] = 4; idx[17] = 5;
	idx[18] = 5; idx[19] = 6;
	idx[20] = 6; idx[21] = 7;
	idx[22] = 7; idx[23] = 4;

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::WireRedCubeMesh,
		topology,
		rasterState, false);
}

void Resources::LoadSphereMesh(int topology, int rasterState)
{
	float radius = 0.5f;		// ���� ������
	int stackCount = 20;		// ���� ����
	int sliceCount = 20;		// ���� ����

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	vector<GraphicsVertex> vertices;

	// �ϱ�
	vertices.push_back(Vector3(0.0f, radius, 0.0f));

	// ������ ���鼭 ������ ����Ѵ� (�ϱ�/���� �������� ���� X)
	for (int y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// ���� ��ġ�� ����
		for (int x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			Vector3 pos;
			pos.x = radius * sinf(phi) * cosf(theta);
			pos.y = radius * cosf(phi);
			pos.z = radius * sinf(phi) * sinf(theta);

			vertices.push_back(pos);
		}
	}

	// ����
	vertices.push_back(Vector3(0.0f, -radius, 0.0f));

	vector<unsigned int> idx;

	// �ϱ� �ε���
	for (int i = 0; i <= sliceCount; ++i)
	{
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ���� �ε���
	int ringVertexCount = sliceCount + 1;
	for (int y = 0; y < stackCount - 2; ++y)
	{
		for (int x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ���� �ε���
	int bottomIndex = static_cast<int>(vertices.size()) - 1;
	int lastRingStartIndex = bottomIndex - ringVertexCount;
	for (int i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::SphereMesh,
		topology,
		rasterState, false);
}

void Resources::LoadWireSphereGreenMesh(int topology, int rasterState)
{
	float radius = 0.5f;		// ���� ������
	int stackCount = 20;		// ���� ����
	int sliceCount = 20;		// ���� ����

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	vector<GraphicsVertex> vertices;

	// �ϱ�
	vertices.push_back(Vector3(0.0f, radius, 0.0f));

	// ������ ���鼭 ������ ����Ѵ� (�ϱ�/���� �������� ���� X)
	for (int y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// ���� ��ġ�� ����
		for (int x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			Vector3 pos;
			pos.x = radius * sinf(phi) * cosf(theta);
			pos.y = radius * cosf(phi);
			pos.z = radius * sinf(phi) * sinf(theta);

			vertices.push_back(pos);
		}
	}

	// ����
	vertices.push_back(Vector3(0.0f, -radius, 0.0f));

	vector<unsigned int> idx;

	// �ϱ� �ε���
	for (int i = 0; i <= sliceCount; ++i)
	{
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ���� �ε���
	int ringVertexCount = sliceCount + 1;
	for (int y = 0; y < stackCount - 2; ++y)
	{
		for (int x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ���� �ε���
	int bottomIndex = static_cast<int>(vertices.size()) - 1;
	int lastRingStartIndex = bottomIndex - ringVertexCount;
	for (int i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	for (auto& vertex : vertices)
	{
		vertex.color = Vector4(0.f, 1.0f, 0.0f, 1.0f);
	}

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::WireGreenSphereMesh,
		topology,
		rasterState, false);
}

void Resources::LoadWireSphereRedMesh(int topology, int rasterState)
{
	float radius = 0.5f;		// ���� ������
	int stackCount = 20;		// ���� ����
	int sliceCount = 20;		// ���� ����

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	vector<GraphicsVertex> vertices;

	// �ϱ�
	vertices.push_back(Vector3(0.0f, radius, 0.0f));

	// ������ ���鼭 ������ ����Ѵ� (�ϱ�/���� �������� ���� X)
	for (int y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// ���� ��ġ�� ����
		for (int x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			Vector3 pos;
			pos.x = radius * sinf(phi) * cosf(theta);
			pos.y = radius * cosf(phi);
			pos.z = radius * sinf(phi) * sinf(theta);

			vertices.push_back(pos);
		}
	}

	// ����
	vertices.push_back(Vector3(0.0f, -radius, 0.0f));

	vector<unsigned int> idx;

	// �ϱ� �ε���
	for (int i = 0; i <= sliceCount; ++i)
	{
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ���� �ε���
	int ringVertexCount = sliceCount + 1;
	for (int y = 0; y < stackCount - 2; ++y)
	{
		for (int x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ���� �ε���
	int bottomIndex = static_cast<int>(vertices.size()) - 1;
	int lastRingStartIndex = bottomIndex - ringVertexCount;
	for (int i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	for (auto& vertex : vertices)
	{
		vertex.color = Vector4(1.f, 0.0f, 0.0f, 1.0f);
	}

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::WireRedSphereMesh,
		topology,
		rasterState, false);
}

void Resources::LoadAxisMesh(int topology, int rasterState)
{
	vector<GraphicsVertex> vertices;
	vertices.push_back(GraphicsVertex(Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(20.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(0.0f, 20.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(0.0f, 0.0f, 20.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)));

	vector<unsigned int> idx;
	idx.push_back(0); idx.push_back(1);
	idx.push_back(2); idx.push_back(3);
	idx.push_back(4); idx.push_back(5);

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::AxisMesh,
		topology,
		rasterState, false);
}

void Resources::LoadGridMesh(int topology, int rasterState)
{
	vector<GraphicsVertex> vertices;
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +19.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +18.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +17.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +16.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +15.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +14.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +13.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +12.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +11.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +10.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +09.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +08.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +07.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +06.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +05.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +04.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +03.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +02.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +01.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +00.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -01.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -02.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -03.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -04.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -05.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -06.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -07.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -08.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -09.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -10.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -11.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -12.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -13.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -14.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -15.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -16.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -17.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -18.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -19.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +19.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +18.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +17.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +16.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +15.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +14.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +13.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +12.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +11.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +10.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +09.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +08.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +07.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +06.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +05.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +04.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +03.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +02.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +01.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +00.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -01.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -02.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -03.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -04.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -05.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -06.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -07.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -08.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -09.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -10.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -11.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -12.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -13.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -14.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -15.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -16.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -17.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -18.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -19.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+19.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+18.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+17.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+16.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+15.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+14.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+13.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+12.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+11.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+10.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+09.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+08.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+07.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+06.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+05.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+04.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+03.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+02.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+01.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+00.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-01.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-02.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-03.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-04.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-05.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-06.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-07.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-08.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-09.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-10.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-11.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-12.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-13.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-14.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-15.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-16.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-17.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-18.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-19.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, +20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+20.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+19.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+18.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+17.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+16.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+15.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+14.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+13.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+12.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+11.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+10.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+09.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+08.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+07.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+06.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+05.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+04.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+03.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+02.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+01.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(+00.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-01.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-02.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-03.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-04.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-05.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-06.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-07.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-08.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-09.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-10.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-11.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-12.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-13.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-14.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-15.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-16.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-17.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-18.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-19.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
	vertices.push_back(GraphicsVertex(Vector3(-20.0f, +0.0f, -20.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));

	vector<unsigned int> idx;
	idx.push_back(0); idx.push_back(41);
	idx.push_back(1); idx.push_back(42);
	idx.push_back(2); idx.push_back(43);
	idx.push_back(3); idx.push_back(44);
	idx.push_back(4); idx.push_back(45);
	idx.push_back(5); idx.push_back(46);
	idx.push_back(6); idx.push_back(47);
	idx.push_back(7); idx.push_back(48);
	idx.push_back(8); idx.push_back(49);
	idx.push_back(9); idx.push_back(50);
	idx.push_back(10); idx.push_back(51);
	idx.push_back(11); idx.push_back(52);
	idx.push_back(12); idx.push_back(53);
	idx.push_back(13); idx.push_back(54);
	idx.push_back(14); idx.push_back(55);
	idx.push_back(15); idx.push_back(56);
	idx.push_back(16); idx.push_back(57);
	idx.push_back(17); idx.push_back(58);
	idx.push_back(18); idx.push_back(59);
	idx.push_back(19); idx.push_back(60);
	idx.push_back(20); idx.push_back(61);
	idx.push_back(21); idx.push_back(62);
	idx.push_back(22); idx.push_back(63);
	idx.push_back(23); idx.push_back(64);
	idx.push_back(24); idx.push_back(65);
	idx.push_back(25); idx.push_back(66);
	idx.push_back(26); idx.push_back(67);
	idx.push_back(27); idx.push_back(68);
	idx.push_back(28); idx.push_back(69);
	idx.push_back(29); idx.push_back(70);
	idx.push_back(30); idx.push_back(71);
	idx.push_back(31); idx.push_back(72);
	idx.push_back(32); idx.push_back(73);
	idx.push_back(33); idx.push_back(74);
	idx.push_back(34); idx.push_back(75);
	idx.push_back(35); idx.push_back(76);
	idx.push_back(36); idx.push_back(77);
	idx.push_back(37); idx.push_back(78);
	idx.push_back(38); idx.push_back(79);
	idx.push_back(39); idx.push_back(80);
	idx.push_back(40); idx.push_back(81);
	idx.push_back(82); idx.push_back(123);
	idx.push_back(83); idx.push_back(124);
	idx.push_back(84); idx.push_back(125);
	idx.push_back(85); idx.push_back(126);
	idx.push_back(86); idx.push_back(127);
	idx.push_back(87); idx.push_back(128);
	idx.push_back(88); idx.push_back(129);
	idx.push_back(89); idx.push_back(130);
	idx.push_back(90); idx.push_back(131);
	idx.push_back(91); idx.push_back(132);
	idx.push_back(92); idx.push_back(133);
	idx.push_back(93); idx.push_back(134);
	idx.push_back(94); idx.push_back(135);
	idx.push_back(95); idx.push_back(136);
	idx.push_back(96); idx.push_back(137);
	idx.push_back(97); idx.push_back(138);
	idx.push_back(98); idx.push_back(139);
	idx.push_back(99); idx.push_back(140);
	idx.push_back(100); idx.push_back(141);
	idx.push_back(101); idx.push_back(142);
	idx.push_back(102); idx.push_back(143);
	idx.push_back(103); idx.push_back(144);
	idx.push_back(104); idx.push_back(145);
	idx.push_back(105); idx.push_back(146);
	idx.push_back(106); idx.push_back(147);
	idx.push_back(107); idx.push_back(148);
	idx.push_back(108); idx.push_back(149);
	idx.push_back(109); idx.push_back(150);
	idx.push_back(110); idx.push_back(151);
	idx.push_back(111); idx.push_back(152);
	idx.push_back(112); idx.push_back(153);
	idx.push_back(113); idx.push_back(154);
	idx.push_back(114); idx.push_back(155);
	idx.push_back(115); idx.push_back(156);
	idx.push_back(116); idx.push_back(157);
	idx.push_back(117); idx.push_back(158);
	idx.push_back(118); idx.push_back(159);
	idx.push_back(119); idx.push_back(160);
	idx.push_back(120); idx.push_back(161);
	idx.push_back(121); idx.push_back(162);
	idx.push_back(122); idx.push_back(163);

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::GridMesh,
		topology,
		rasterState, false);
}

void Resources::LoadScreenMesh(int topology, int rasterState)
{
	vector<GraphicsVertex> vertices;
	vertices.push_back(GraphicsVertex(Vector3(-1.0f, -1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.f, 1.f)));
	vertices.push_back(GraphicsVertex(Vector3(-1.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.f, 0.f)));
	vertices.push_back(GraphicsVertex(Vector3(1.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.f, 0.f)));
	vertices.push_back(GraphicsVertex(Vector3(-1.0f, -1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.f, 1.f)));
	vertices.push_back(GraphicsVertex(Vector3(1.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.f, 0.f)));
	vertices.push_back(GraphicsVertex(Vector3(1.0f, -1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.f, 1.f)));

	vector<unsigned int> idx;
	idx.push_back(0);
	idx.push_back(1);
	idx.push_back(2);
	idx.push_back(3);
	idx.push_back(4);
	idx.push_back(5);

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::ScreenMesh,
		topology,
		rasterState, false);
}

void Resources::LoadRectMesh(int topology, int rasterState)
{
	vector<GraphicsVertex> vertices;
	vertices.push_back(GraphicsVertex(Vector3(-1.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.f, 1.f)));
	vertices.push_back(GraphicsVertex(Vector3(-1.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.f, 0.f)));
	vertices.push_back(GraphicsVertex(Vector3(1.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.f, 0.f)));
	vertices.push_back(GraphicsVertex(Vector3(-1.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.f, 1.f)));
	vertices.push_back(GraphicsVertex(Vector3(1.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.f, 0.f)));
	vertices.push_back(GraphicsVertex(Vector3(1.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.f, 1.f)));

	vector<unsigned int> idx;
	idx.push_back(0);
	idx.push_back(1);
	idx.push_back(2);
	idx.push_back(3);
	idx.push_back(4);
	idx.push_back(5);

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::RectMesh,
		topology,
		rasterState, false);
}

void Resources::LoadRectWireMesh(int topology, int rasterState)
{
	float x = 0.5f;
	float y = 0.5f;
	float z = 0.5f;
	
	vector<GraphicsVertex> vertices(4);
	vertices[0] = GraphicsVertex(Vector3(-x, 0.f, -z), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[1] = GraphicsVertex(Vector3(-x, 0.f, +z), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[2] = GraphicsVertex(Vector3(+x, 0.f, +z), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[3] = GraphicsVertex(Vector3(+x, 0.f, -z), Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	vector<unsigned int> idx;
	idx.push_back(0);
	idx.push_back(1);

	idx.push_back(1);
	idx.push_back(2);
	
	idx.push_back(2);
	idx.push_back(3);

	idx.push_back(3);
	idx.push_back(0);

	
	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::WireRectMesh,
		topology,
		rasterState, false);
}

void Resources::LoadWaveMesh(int topology, int rasterState)
{
	vector<GraphicsVertex> vertices;
	vertices.push_back(GraphicsVertex(Vector3(-2.0f, 0.0f, 2.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.f, 1.f)));
	vertices.push_back(GraphicsVertex(Vector3(2.0f, 0.0f, 2.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.f, 1.f)));
	vertices.push_back(GraphicsVertex(Vector3(-2.0f, 0.0f, -2.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.f, 0.f)));
	vertices.push_back(GraphicsVertex(Vector3(-2.0f, 0.0f, -2.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.f, 0.f)));
	vertices.push_back(GraphicsVertex(Vector3(2.0f, 0.0f, 2.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.f, 1.f)));
	vertices.push_back(GraphicsVertex(Vector3(2.0f, 0.0f, -2.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.f, 0.f)));

	vector<unsigned int> idx;
	idx.push_back(0);
	idx.push_back(1);
	idx.push_back(2);
	idx.push_back(3);
	idx.push_back(4);
	idx.push_back(5);

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::WaveMesh,
		topology,
		rasterState, false);
}

// �������� �������� �������� ������� (���������� ���������)
void Resources::LoadParticleMesh(int topology, int rasterState)
{
	int m_VertexCount = _particleInfo->_currentParticleCount * 6;
	int m_IndexCount = m_VertexCount;

	vector<GraphicsVertex> vertices(m_VertexCount);
	vector<unsigned int> idx(m_IndexCount);

	// �ε��� �迭�� �ʱ�ȭ�մϴ�.
	for (int i = 0; i < m_IndexCount; i++)
	{
		idx[i] = i;
	}
	int index = 0;

	for (int i = 0; i < _particleInfo->_currentParticleCount; i++)
	{
		// ���� �Ʒ�.
		vertices[index] = GraphicsVertex(Vector3(_particleInfo->_particleList[i]->positionX - _particleInfo->_particleSize, _particleInfo->_particleList[i]->positionY+3.0f, _particleInfo->_particleList[i]->positionZ - _particleInfo->_particleSize),
			Vector4(_particleInfo->_particleList[i]->red, _particleInfo->_particleList[i]->green, _particleInfo->_particleList[i]->blue, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
		index++;

		// ���� ��.
		vertices[index] = GraphicsVertex(Vector3(_particleInfo->_particleList[i]->positionX - _particleInfo->_particleSize, _particleInfo->_particleList[i]->positionY + 3.0f, _particleInfo->_particleList[i]->positionZ + _particleInfo->_particleSize),
			Vector4(_particleInfo->_particleList[i]->red, _particleInfo->_particleList[i]->green, _particleInfo->_particleList[i]->blue, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
		index++;

		// ������ �Ʒ�.
		vertices[index] = GraphicsVertex(Vector3(_particleInfo->_particleList[i]->positionX + _particleInfo->_particleSize, _particleInfo->_particleList[i]->positionY + 3.0f, _particleInfo->_particleList[i]->positionZ - _particleInfo->_particleSize),
			Vector4(_particleInfo->_particleList[i]->red, _particleInfo->_particleList[i]->green, _particleInfo->_particleList[i]->blue, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
		index++;

		// ������ �Ʒ�.
		vertices[index] = GraphicsVertex(Vector3(_particleInfo->_particleList[i]->positionX + _particleInfo->_particleSize, _particleInfo->_particleList[i]->positionY + 3.0f, _particleInfo->_particleList[i]->positionZ - _particleInfo->_particleSize),
			Vector4(_particleInfo->_particleList[i]->red, _particleInfo->_particleList[i]->green, _particleInfo->_particleList[i]->blue, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
		index++;

		// ���� ��.
		vertices[index] = GraphicsVertex(Vector3(_particleInfo->_particleList[i]->positionX - _particleInfo->_particleSize, _particleInfo->_particleList[i]->positionY + 3.0f, _particleInfo->_particleList[i]->positionZ + _particleInfo->_particleSize),
			Vector4(_particleInfo->_particleList[i]->red, _particleInfo->_particleList[i]->green, _particleInfo->_particleList[i]->blue, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
		index++;

		// ������ ��.
		vertices[index] = GraphicsVertex(Vector3(_particleInfo->_particleList[i]->positionX + _particleInfo->_particleSize, _particleInfo->_particleList[i]->positionY + 3.0f, _particleInfo->_particleList[i]->positionZ + _particleInfo->_particleSize),
			Vector4(_particleInfo->_particleList[i]->red, _particleInfo->_particleList[i]->green, _particleInfo->_particleList[i]->blue, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
		index++;
	}

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::ParticleMesh,
		topology,
		rasterState, true);

}

void Resources::LoadCircleMesh(int topology, int rasterState)
{
	int n = 20;

	vector<GraphicsVertex> vertices(n * 3);

	float deltaTheta = 2 * 3.141592 / n;

	for (int i = 0; i < n; i++)
	{
		float theta = i * deltaTheta;
		int index = 3 * i;

		vertices[index + 0] = GraphicsVertex(Vector3(0, 0, 0), Vector4(1.0f, 0.f, 0.f, 1.0f));
		vertices[index + 1] = GraphicsVertex(Vector3(cos(theta + deltaTheta), 0, sin(theta + deltaTheta)), Vector4(1.0f, 0.f, 0.f, 1.0f));
		vertices[index + 2] = GraphicsVertex(Vector3(cos(theta), 0, sin(theta)), Vector4(1.0f, 0.f, 0.f, 1.0f));
	}

	vector<unsigned int> idx(n * 3);
	for (int i = 0; i < n * 3; i++)
	{
		idx[i] = i;
	}

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::CircleMesh,
		topology,
		rasterState, false);
}

void Resources::LoadOceanMesh(int topology, int rasterState)
{
	int terrainHeight = 256;
	int terrainWidth = 256;

	// ���� �޽��� ���� ���� ����մϴ�.
	int m_VertexCount = (terrainWidth - 1) * (terrainHeight - 1) * 6;

	// �ε��� ���� ������ ���� ���� �����մϴ�.
	int m_IndexCount = m_VertexCount;

	int index = 0;

	vector<GraphicsVertex> vertices(m_VertexCount);
	vector<unsigned int> idx(m_IndexCount);

	// ���� �����ͷ� ���� �� �ε��� �迭�� �ε��մϴ�.
	for (int j = 0; j < (terrainHeight - 1); j++)
	{
		for (int i = 0; i < (terrainWidth - 1); i++)
		{
			// LINE 1
			// ���� ��.
			float positionX = (float)i;
			float positionZ = (float)(j + 1);

			vertices[index]=GraphicsVertex(Vector3(positionX, 0.0f, positionZ), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
			idx[index] = index;
			index++;

			// ������ ��.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);
			
			vertices[index] = GraphicsVertex(Vector3(positionX, 0.0f, positionZ), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
			idx[index] = index;
			index++;

			// ���� �Ʒ�.
			positionX = (float)i;
			positionZ = (float)j;

			vertices[index] = GraphicsVertex(Vector3(positionX, 0.0f, positionZ), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
			idx[index] = index;
			index++;

				// ���� �Ʒ�.
			positionX = (float)i;
			positionZ = (float)j;

			vertices[index] = GraphicsVertex(Vector3(positionX, 0.0f, positionZ), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
			idx[index] = index;
			index++;


			// ������ ��.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index] = GraphicsVertex(Vector3(positionX, 0.0f, positionZ), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
			idx[index] = index;
			index++;

			// LINE 3
			// ������ �Ʒ�.
			positionX = (float)(i + 1);
			positionZ = (float)j;

			vertices[index] = GraphicsVertex(Vector3(positionX, 0.0f, positionZ), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
			idx[index] = index;
			index++;
		}
	}

	GraphicsEngineManager::GetInstance()->SetMesh(
		vertices,
		idx,
		(int)MeshName::OceanMesh,
		topology,
		rasterState, false);
}

shared_ptr<MeshData> Resources::LoadFBX(const wstring& path, int topology, int rasterizerState, int pass)
{
	//// fbx �� �غ�..
	//shared_ptr<ParserBase> fbxLoader = ParserBase::Create();
	//
	//// fbx �� �ε�
	//shared_ptr<NoobData::FbxModel> fbxModel = fbxLoader->LoadFbx(path);

	//// mesh�������� ��Ƴ��� �ٰ��̴�.
	//// material ������, � �޽ó���(�ؽ�)�� ������ �ִ�����.. �ϴ� ������? ���߿� �ִϸ��̼� ������ ������
	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	//// �׷��Ƚ� �� ���ҽ��Ŵ����� �������� �޽��� �޽ù��ۿ� ���� ������ ����
	//for (int i = 0; i < fbxModel->fbxMeshInfos.size(); i++)
	//{
	//	vector<GraphicsFbxVertex> graphicsVertices;
	//	for (int j = 0; j < fbxModel->fbxMeshInfos[i].vertices.size(); j++)
	//	{
	//		GraphicsFbxVertex graphicsVertex;
	//		graphicsVertex.pos = fbxModel->fbxMeshInfos[i].vertices[j].pos;
	//		graphicsVertex.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);			//	FBX���� ���� �����ʱ⿡ �ϴ�..
	//		graphicsVertex.uv = fbxModel->fbxMeshInfos[i].vertices[j].uv;
	//		graphicsVertex.normal = fbxModel->fbxMeshInfos[i].vertices[j].normal;
	//		graphicsVertex.tangent = fbxModel->fbxMeshInfos[i].vertices[j].tangent;
	//		graphicsVertex.weights = fbxModel->fbxMeshInfos[i].vertices[j].weights;
	//		graphicsVertex.indices = fbxModel->fbxMeshInfos[i].vertices[j].weightsIndices;
	//		graphicsVertices.push_back(graphicsVertex);
	//	}

	//	// �ش� �޽��� vertices, indices, name �׷��Ƚ� ResourceManager�����
	//	GraphicsEngineManager::GetInstance()->SetFbxMesh(
	//		graphicsVertices/*fbxModel->fbxMeshInfos[i].vertices*/,
	//		fbxModel->fbxMeshInfos[i].indices,
	//		fbxModel->fbxMeshInfos[i].name,
	//		topology,
	//		rasterizerState);
	//	
	//	// ���� �޽� �����Ϳ� �ش� �޽ø� �����Ұž�
	//	vector<shared_ptr<Material>> graphicsMaterials;

	//	for (int j = 0; j < fbxModel->fbxMeshInfos[i].materials.size(); j++)
	//	{
	//		shared_ptr<Material> graphicsMaterial = make_shared<Material>();

	//		graphicsMaterial->isDiffuse = fbxModel->fbxMeshInfos[i].materials[j].isDiffuse;
	//		graphicsMaterial->isNormal = fbxModel->fbxMeshInfos[i].materials[j].isNormal;
	//		graphicsMaterial->isSpecular = fbxModel->fbxMeshInfos[i].materials[j].isSpecular;
	//		graphicsMaterial->ambient = fbxModel->fbxMeshInfos[i].materials[j].ambient;
	//		graphicsMaterial->diffuse = fbxModel->fbxMeshInfos[i].materials[j].diffuse;
	//		graphicsMaterial->specular = fbxModel->fbxMeshInfos[i].materials[j].specular;
	//		graphicsMaterial->diffuseTexture = fbxModel->fbxMeshInfos[i].materials[j].diffuseTexName;
	//		graphicsMaterial->normalTexture = fbxModel->fbxMeshInfos[i].materials[j].normalTexName;
	//		graphicsMaterial->specularTexture = fbxModel->fbxMeshInfos[i].materials[j].specularTexName;
	//		
	//		graphicsMaterials.push_back(graphicsMaterial);
	//	}


	//	if (fbxModel->fbxMeshInfos[i].isAnimation)
	//	{
	//		vector<shared_ptr<AnimClipInfo>> graphicsAnimation;

	//		for (int clipCnt = 0; clipCnt < fbxModel->animationClipList.size(); clipCnt++)
	//		{
	//			shared_ptr<AnimClipInfo> graphicsAnim = make_shared<AnimClipInfo>();
	//			graphicsAnim->name = fbxModel->animationClipList[clipCnt]->name;
	//			graphicsAnim->ticksPerFrame = fbxModel->animationClipList[clipCnt]->ticksPerFrame;
	//			graphicsAnim->totalFrame = fbxModel->animationClipList[clipCnt]->totalFrame;
	//			graphicsAnim->startFrame = fbxModel->animationClipList[clipCnt]->startFrame;
	//			graphicsAnim->endFrame = fbxModel->animationClipList[clipCnt]->endFrame;
	//			graphicsAnim->keyFrames.resize(fbxModel->animationClipList[clipCnt]->keyFrames.size());

	//			for (int boneCnt = 0; boneCnt < fbxModel->animationClipList[clipCnt]->keyFrames.size(); boneCnt++)
	//			{
	//				for (int frameCnt = 0; frameCnt < fbxModel->animationClipList[clipCnt]->keyFrames[boneCnt].size(); frameCnt++)
	//				{
	//					shared_ptr<KeyFrameInfo> frameInfo = make_shared<KeyFrameInfo>();
	//					frameInfo->time = fbxModel->animationClipList[clipCnt]->keyFrames[boneCnt][frameCnt]->time;
	//					frameInfo->scale = fbxModel->animationClipList[clipCnt]->keyFrames[boneCnt][frameCnt]->scale;
	//					frameInfo->rotation = fbxModel->animationClipList[clipCnt]->keyFrames[boneCnt][frameCnt]->rotation;
	//					frameInfo->translate = fbxModel->animationClipList[clipCnt]->keyFrames[boneCnt][frameCnt]->translation;

	//					graphicsAnim->keyFrames[boneCnt].push_back(frameInfo);
	//				}
	//			}

	//			graphicsAnimation.push_back(graphicsAnim);
	//		}

	//		vector<shared_ptr<BoneInfo>> graphicsBoneInfos;

	//		for (int boneInfoCnt = 0; boneInfoCnt < fbxModel->fbxBoneInfo.size(); boneInfoCnt++)
	//		{
	//			shared_ptr<BoneInfo> graphicsBoneInfo = make_shared<BoneInfo>();
	//			graphicsBoneInfo->boneName = fbxModel->fbxBoneInfo[boneInfoCnt]->boneName;
	//			graphicsBoneInfo->parentIndex = fbxModel->fbxBoneInfo[boneInfoCnt]->parentIndex;
	//			graphicsBoneInfo->matOffset = fbxModel->fbxBoneInfo[boneInfoCnt]->matOffset;
	//			graphicsBoneInfos.push_back(graphicsBoneInfo);
	//		}

	//		meshData->SetMeshRenderInfo(fbxModel->fbxMeshInfos[i].name, graphicsMaterials, pass, graphicsAnimation, graphicsBoneInfos);
	//	}

	//	// �޽� �����Ϳ� �ش� �޽��� �̸��� ���͸��� ���
	//	if (!fbxModel->fbxMeshInfos[i].isAnimation)
	//	{
	//		meshData->SetMeshRenderInfo(fbxModel->fbxMeshInfos[i].name, graphicsMaterials, pass);
	//	}
	//}
	//
	return meshData;
}

shared_ptr<MeshData> Resources::LoadASE(string path, int topology, int rasterizerState)
{
	shared_ptr<MeshData> findMeshData = aseMeshData[path];
	if (findMeshData)
		return findMeshData;

	// ase �� �ε�
	shared_ptr<NoobData::ASEModel> aseModel = _aseParser->Load(path);

	// mesh�������� ��Ƴ��� �ٰ��̴�.
	// material ������, � �޽ó���(�ؽ�)�� ������ �ִ�����.. �ϴ� ������? ���߿� �ִϸ��̼� ������ ������
	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	hash<string> hasher;

	// ASEModel�� materials���� Material�� ���������� ��Ƴ��°��� push ���ش�.
	for (int materialCnt = 0; materialCnt < aseModel->materials.size(); materialCnt++)
	{
		shared_ptr<MaterialInfo> materialInfo = make_shared<MaterialInfo>();
		materialInfo->ambient = aseModel->materials[materialCnt]->ambient;
		materialInfo->diffuse = aseModel->materials[materialCnt]->diffuse;
		materialInfo->specular = aseModel->materials[materialCnt]->specular;
		materialInfo->isDiffuse = aseModel->materials[materialCnt]->isDiffuse;
		materialInfo->isNormal = aseModel->materials[materialCnt]->isNormal;
		materialInfo->isSpecular = aseModel->materials[materialCnt]->isSpecular;
		materialInfo->diffuseTexture = aseModel->materials[materialCnt]->diffuseTexName;
		materialInfo->normalTexture = aseModel->materials[materialCnt]->normalTexName;
		materialInfo->specularTexture = aseModel->materials[materialCnt]->specularTexName;

		GraphicsEngineManager::GetInstance()->SetMaterialInfo(materialInfo);
	}

	for (int i = 0; i < aseModel->meshInfos.size(); i++)
	{
		vector<GraphicsVertex> graphicsVertices;
		for (int j = 0; j < aseModel->meshInfos[i]->vertices.size(); j++)
		{
			GraphicsVertex graphicsVertex;
			graphicsVertex.pos = aseModel->meshInfos[i]->vertices[j].pos;
			graphicsVertex.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			graphicsVertex.uv = aseModel->meshInfos[i]->vertices[j].uv;
			graphicsVertex.normal = aseModel->meshInfos[i]->vertices[j].normal;
			graphicsVertex.tangent = aseModel->meshInfos[i]->vertices[j].tangent;

			for (int weightCnt = 0; weightCnt < 8; weightCnt++)
			{
				graphicsVertex.weights[weightCnt] = aseModel->meshInfos[i]->vertices[j].weights[weightCnt];
				graphicsVertex.boneIndices[weightCnt] = aseModel->meshInfos[i]->vertices[j].boneIndices[weightCnt];
			}
			graphicsVertices.push_back(graphicsVertex);
		}

		// �ش� �޽��� vertices, indices, name �׷��Ƚ� ResourceManager�����
		GraphicsEngineManager::GetInstance()->SetMesh(
			graphicsVertices,
			aseModel->meshInfos[i]->indices,
			aseModel->meshInfos[i]->meshName,
			topology,
			rasterizerState, false);

		// �ش� �޽��� BoundingBox�� ���ӿ��� Resources�� ���
		// �ٵ� ����׵�ο� ���ַ��� �׷��Ƚ����� ����ؾ��ϳ�..?
		// ���ӿ�����Ʈ ���鼭 �ش� �ٿ���ڽ� �����ͼ� �����Ʈ������ ��TM �����ְ� 
		// ī�޶��� ���������ϰ� ���ؼ� ����ť��(ť�¾ƴ�����) ������ ���� ���ش�?
		CreateMeshBoundingCube(graphicsVertices, aseModel->meshInfos[i]->meshName);

		//vector<shared_ptr<Material>> graphicsMaterials;

		/*for (int j = 0; j < aseModel->meshInfos[i]->materials.size(); j++)
		{
			shared_ptr<Material> graphicsMaterial = make_shared<Material>();

			graphicsMaterial->isDiffuse = aseModel->meshInfos[i]->materials[j]->isDiffuse;
			graphicsMaterial->isNormal = aseModel->meshInfos[i]->materials[j]->isNormal;
			graphicsMaterial->isSpecular = aseModel->meshInfos[i]->materials[j]->isSpecular;
			graphicsMaterial->ambient = aseModel->meshInfos[i]->materials[j]->ambient;
			graphicsMaterial->diffuse = aseModel->meshInfos[i]->materials[j]->diffuse;
			graphicsMaterial->specular = aseModel->meshInfos[i]->materials[j]->specular;
			graphicsMaterial->diffuseTexture = aseModel->meshInfos[i]->materials[j]->diffuseTexName;
			graphicsMaterial->normalTexture = aseModel->meshInfos[i]->materials[j]->normalTexName;
			graphicsMaterial->specularTexture = aseModel->meshInfos[i]->materials[j]->specularTexName;

			graphicsMaterials.push_back(graphicsMaterial);
		}*/


		// �ִϸ��̼��� �����ϰ� �ִ���
		if (aseModel->meshInfos[i]->isAnimation)
		{
			vector<shared_ptr<AnimationClipInfo>> graphicsAnimInfos;

			// �ִϸ��̼� ������ŭ..
			for (int animCnt = 0; animCnt < aseModel->meshInfos[i]->animationClips.size(); animCnt++)
			{
				shared_ptr<AnimationClipInfo> graphicsAnim = make_shared<AnimationClipInfo>();
				graphicsAnim->nodename = hasher(aseModel->meshInfos[i]->animationClips[animCnt]->nodename);
				graphicsAnim->animName = aseModel->meshInfos[i]->animationClips[animCnt]->animName;

				for (auto& pos : aseModel->meshInfos[i]->animationClips[animCnt]->position)
				{
					shared_ptr<Animation_p> animation_p = make_shared<Animation_p>();
					animation_p->tick = pos->tick;
					animation_p->pos = pos->pos;
					graphicsAnim->position.push_back(animation_p);
				}
				for (auto& rot : aseModel->meshInfos[i]->animationClips[animCnt]->rotation)
				{
					shared_ptr<Animation_r> animation_r = make_shared<Animation_r>();
					animation_r->tick = rot->tick;
					animation_r->rotQT = rot->rotQT;
					graphicsAnim->rotation.push_back(animation_r);
				}
				for (auto& scale : aseModel->meshInfos[i]->animationClips[animCnt]->scale)
				{
					shared_ptr<Animation_s> animation_s = make_shared<Animation_s>();
					animation_s->time = scale->time;
					animation_s->scale = scale->scale;
					animation_s->scaleaxis = scale->scaleaxis;
					animation_s->scaleaxisang = scale->scaleaxisang;
					graphicsAnim->scale.push_back(animation_s);
				}
				graphicsAnim->firstFrame = aseModel->frameData->firstFrame;
				graphicsAnim->totalFrame = aseModel->frameData->lastFrame;
				graphicsAnim->frameSpeed = aseModel->frameData->frameSpeed;
				//graphicsAnim->ticksPerFrame = aseModel->frameData->ticksPerFrame;

				graphicsAnimInfos.push_back(graphicsAnim);
			}


			vector<shared_ptr<BoneInfo>> graphicsBoneInfos;

			for (int boneInfoCnt = 0; boneInfoCnt < aseModel->meshInfos[i]->boneInfos.size(); boneInfoCnt++)
			{
				shared_ptr<BoneInfo> graphicsBoneInfo = make_shared<BoneInfo>();
				graphicsBoneInfo->boneName = hasher(aseModel->meshInfos[i]->boneInfos[boneInfoCnt]->boneName);
				graphicsBoneInfos.push_back(graphicsBoneInfo);
			}

			meshData->SetMeshRenderInfo(aseModel->meshInfos[i]->meshName, aseModel->meshInfos[i]->gameObjName, aseModel->meshInfos[i]->parentName, aseModel->meshInfos[i]->materialID, aseModel->meshInfos[i]->nodeTM, graphicsAnimInfos, graphicsBoneInfos);
		}

		vector<shared_ptr<BoneInfo>> graphicsBoneInfos;

		for (int boneInfoCnt = 0; boneInfoCnt < aseModel->meshInfos[i]->boneInfos.size(); boneInfoCnt++)
		{
			shared_ptr<BoneInfo> graphicsBoneInfo = make_shared<BoneInfo>();
			graphicsBoneInfo->boneName = hasher(aseModel->meshInfos[i]->boneInfos[boneInfoCnt]->boneName);
			graphicsBoneInfos.push_back(graphicsBoneInfo);
		}

		// �޽� �����Ϳ� �ش� �޽��� �̸��� ���͸��� ���
		if (!aseModel->meshInfos[i]->isAnimation)
		{
			meshData->SetMeshRenderInfo(aseModel->meshInfos[i]->meshName, aseModel->meshInfos[i]->gameObjName, aseModel->meshInfos[i]->parentName, aseModel->meshInfos[i]->materialID, aseModel->meshInfos[i]->nodeTM, graphicsBoneInfos);
		}
	}

	meshData->SetMeshMinMaxPos(aseModel->minPos, aseModel->maxPos);

	aseMeshData[path] = meshData;

	return meshData;
}

unsigned int Resources::GetMaterialID()
{
	return _aseParser->GetMaterialID();
}

void Resources::PlusMaterialID()
{
	_aseParser->PlusMaterialID();
}

void Resources::Renewal()
{
	GraphicsEngineManager::GetInstance()->RenewalMaterials();
}

void Resources::SetMaterial(shared_ptr<MaterialInfo> materialInfo)
{
	GraphicsEngineManager::GetInstance()->SetMaterialInfo(materialInfo);
}

void Resources::SetCubeMapTexture(int materialID, wstring cubeMapTexture)
{
	GraphicsEngineManager::GetInstance()->SetCubeMapTextureInfo(materialID, cubeMapTexture);
}

void Resources::SetDiffuseMapTexture(int materialID, wstring diffuseTexture)
{
	GraphicsEngineManager::GetInstance()->SetDiffuseMapTextureInfo(materialID, diffuseTexture);
}

void Resources::SetNormalMapTexture(int materialID, wstring normalMapTexture)
{
	GraphicsEngineManager::GetInstance()->SetNormalMapTextureInfo(materialID, normalMapTexture);
}

// Ŭ���̾�Ʈ���� �־��� info�� �������� �޾Ƽ� ����ϱ� ���� �־��ش�.
void Resources::SetParticleInfo(shared_ptr<ParticleInfo> particleInfo)
{
	_particleInfo = particleInfo;
}

void Resources::InputEntryDir(std::string& path)
{
	std::vector<std::string> _dirs;

	GetAllDir(path, _dirs);

	//_bankNames = _dirs;
}

void Resources::GetAllDir(std::string& path, std::vector<std::string>& dirs)
{
	// ���� ��θ� ���ؼ� �������� ���, �ش� ����� ��� �͵��� ��´�.
	// ���� �������� �߸���.

	// ��� �͵��� ��� �����̳�
	std::vector<std::string> _tempNames;

	GetContents(path, _tempNames);

	unsigned int _currentFolderCount = 0;
	bool _dirName;

	std::queue<std::string> _tempFolder;

	for (std::string _nowName : _tempNames)
	{
		_dirName = true;

		// ������ ���� ������ �Ѿ��
		if (_nowName == "." || _nowName == "..")
		{
			continue;
		}

		for (size_t i = 0; i < _nowName.size(); i++)
		{
			if (_nowName.at(i) == '.')
			{
				_dirName = false;
				break;
			}
		}

		if (_dirName == true)
		{
			_tempFolder.push(_nowName);
			_currentFolderCount++;
		}
		else
		{
			std::string _newFileName = path + _nowName;
			StorePath(_newFileName);
		}
	}

	// ���� ���� ���� ���� 1�� �̻��̸�
	if (_currentFolderCount >= 1)
	{

		for (unsigned int i = 0; i < _currentFolderCount; i++)
		{

			std::string _newPath = path;
			_newPath.append(_tempFolder.front() + '/');
			_tempFolder.pop();

			dirs.push_back(_newPath);

			GetAllDir(_newPath, dirs);
		}
	}
	else
	{
		return;
	}
}

void Resources::GetContents(std::string& path, std::vector<std::string>& dirs)
{
	std::string _pattern;
	_pattern.assign(path.begin(), path.end());
	_pattern.append("\\*");
	WIN32_FIND_DATA _data;
	HANDLE _hFind;

	if ((_hFind = FindFirstFile(_pattern.c_str(), &_data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::string _wstr(_data.cFileName);
			//std::string _str(_wstr.begin(), _wstr.end());

			dirs.push_back(_wstr);

		} while (FindNextFile(_hFind, &_data) != 0);

		FindClose(_hFind);
	}
}

void Resources::StorePath(std::string& path)
{
	std::string _sample;

	for (size_t i = path.size() - 1; i > 0; i--)
	{
		_sample += path.at(i);

		// Ȯ���� ���� ��� ���� �ڿ� �������� �˻��Ѵ�.
		if (path.at(i) == '.')
		{
			ReverseString(_sample);

			if (_sample == ".bank" || _sample == ".strings.bank")
			{
				_bankNames.push_back(path);
				return;
			}
			
		}
	}
}

void Resources::ReverseString(std::string& str)
{
	char _temp;
	for (size_t i = 0; i < str.size() / 2; i++)
	{
		_temp = str[i];
		str[i] = str[(str.size() - 1) - i];
		str[(str.size() - 1) - i] = _temp;
	}
}
