#pragma once
#include "Mesh.h"
#include "RasterizerState.h"

class Texture;
struct MaterialCollection;

class ResourceManager
{
public:
	ResourceManager() {};
	~ResourceManager() {};

private:
	static shared_ptr<ResourceManager> resourceManager;

public:
	static std::vector<shared_ptr<MaterialInfo>> materials;		// Material���� ���� ���� parser���� ������ push ����

	static shared_ptr<ResourceManager> GetInstance();

	static cbMaterial cbMaterialBuffer;							// texture�̸��� �ȵ���. 

private:
	map<wstring, shared_ptr<Texture>> _textures;
	wstring _path;
	wstring _extension;

	map<size_t, shared_ptr<Mesh>> _meshs;						// Mesh Name������� ��� ���� mesh�� ����(string->int)
	vector<shared_ptr<Mesh>> _particle;

public:
	void Init();

	void Renewal();

	void Release();

	void CreateTexture(const wstring& name, const wstring& path);

	// �ؽ��ĸ� �����µ� ������ �ִ��� ���� ������ ����
	shared_ptr<Texture> GetTexture(const wstring& name);

	// �޽ø� ����
	const shared_ptr<Mesh>& GetMesh(size_t name);
	const shared_ptr<Mesh>& GetParticleMesh(int name);

	void AddMesh(size_t name, shared_ptr<Mesh> mesh);
	void AddParticle(shared_ptr<Mesh> particle);

	void ReSetParticle(std::vector<int>& deleteindex);

	// vertices, indices... ���� �޾Ƽ� buffer ������ ���ش�.
	// fbxmodel�� �ִ� mesh��!�� ���� ������ϴϱ�..
	template <typename T>
	void CreateMesh(vector<T> vertices, vector<unsigned int> indices, size_t name, int topology, int rasterizerState, bool isParticle);

};

// �޽ø� �������
template<typename T>
inline void ResourceManager::CreateMesh(vector<T> vertices, vector<unsigned int> indices, size_t name, int topology, int rasterizerState, bool isParticle)
{
	shared_ptr<Mesh> mesh = make_shared<Mesh>();

	// �ϴ� �ӽ�..
	if (topology == 0)
		mesh->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else if (topology == 1)
		mesh->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	if (rasterizerState == 0)
		mesh->SetRenderState(GraphicsEngineAPI::GetInstance()->GetWireClass()->GetRasterizerState());
	else if (rasterizerState == 1)
		mesh->SetRenderState(GraphicsEngineAPI::GetInstance()->GetSolidClass()->GetRasterizerState());

	mesh->CreateVertexBuffer(vertices);
	mesh->CreateIndexBuffer(indices);
	mesh->SetIndexBufferSize(indices.size());
	mesh->stride = sizeof(T);

	if (!isParticle)
		AddMesh(name, mesh);
	else
		AddParticle(mesh);
}

