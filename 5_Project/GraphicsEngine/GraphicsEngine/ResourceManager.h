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
	static std::vector<shared_ptr<MaterialInfo>> materials;		// Material들을 모은 변수 parser에서 받으면 push 해줌

	static shared_ptr<ResourceManager> GetInstance();

	static cbMaterial cbMaterialBuffer;							// texture이름이 안들어간다. 

private:
	map<wstring, shared_ptr<Texture>> _textures;
	wstring _path;
	wstring _extension;

	map<size_t, shared_ptr<Mesh>> _meshs;						// Mesh Name순서대로 담고 여러 mesh도 담긴다(string->int)
	vector<shared_ptr<Mesh>> _particle;

public:
	void Init();

	void Renewal();

	void Release();

	void CreateTexture(const wstring& name, const wstring& path);

	// 텍스쳐를 얻어오는데 있으면 있던거 쓰고 없으면 생성
	shared_ptr<Texture> GetTexture(const wstring& name);

	// 메시를 얻어옴
	const shared_ptr<Mesh>& GetMesh(size_t name);
	const shared_ptr<Mesh>& GetParticleMesh(int name);

	void AddMesh(size_t name, shared_ptr<Mesh> mesh);
	void AddParticle(shared_ptr<Mesh> particle);

	void ReSetParticle(std::vector<int>& deleteindex);

	// vertices, indices... 등을 받아서 buffer 생성을 해준다.
	// fbxmodel에 있는 mesh들!을 전부 해줘야하니까..
	template <typename T>
	void CreateMesh(vector<T> vertices, vector<unsigned int> indices, size_t name, int topology, int rasterizerState, bool isParticle);

};

// 메시를 만들어줌
template<typename T>
inline void ResourceManager::CreateMesh(vector<T> vertices, vector<unsigned int> indices, size_t name, int topology, int rasterizerState, bool isParticle)
{
	shared_ptr<Mesh> mesh = make_shared<Mesh>();

	// 일단 임시..
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

