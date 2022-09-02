#include "pch.h"
#include "GraphicsEngineAPI.h"
#include "ResourceManager.h"
#include "DDSTextureLoader.h"
#include "VertexDefine.h"
#include "RasterizerState.h"
#include "Texture.h"

shared_ptr<ResourceManager> ResourceManager::resourceManager = nullptr;
cbMaterial ResourceManager::cbMaterialBuffer;
std::vector<shared_ptr<MaterialInfo>> ResourceManager::materials;

shared_ptr<ResourceManager> ResourceManager::GetInstance()
{
	if (resourceManager == nullptr)
		resourceManager = make_shared<ResourceManager>();

	return resourceManager;
}

void ResourceManager::Init()
{
	_path = L"Data/Texture/";
	//_extension = L".dds";
}

// 게임 클라이언트에서 사용할것임
void ResourceManager::Renewal()
{
	// 가지고 있는 전체 material을 Cbuffer에 넣어준다.
	for (size_t i = 0; i < materials.size(); i++)
	{
		cbMaterialBuffer.gMaterials[i].gMaterialAmbient = materials[i]->ambient;
		cbMaterialBuffer.gMaterials[i].gMaterialDiffuse = materials[i]->diffuse;
		cbMaterialBuffer.gMaterials[i].gMaterialSpecular= materials[i]->specular;
		cbMaterialBuffer.gMaterials[i].gMaterialReflection = materials[i]->reflection;
		cbMaterialBuffer.gMaterials[i].isDiffuseTexture= materials[i]->isDiffuse;
		cbMaterialBuffer.gMaterials[i].isNormalTexture= materials[i]->isNormal;
		cbMaterialBuffer.gMaterials[i].isSpecularTexture= materials[i]->isSpecular;
		cbMaterialBuffer.gMaterials[i].isReflectionTexture = materials[i]->isReflection;
		cbMaterialBuffer.gMaterials[i].isUtil = materials[i]->isUtil;
	}
}

void ResourceManager::Release()
{
	_textures.clear();
	_meshs.clear();
}


void ResourceManager::CreateTexture(const wstring& name, const wstring& path)
{
	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->Init(path);									// 텍스쳐 srv 생성

	_textures.insert(make_pair(name, texture));
}

shared_ptr<Texture> ResourceManager::GetTexture(const wstring& name)
{
	auto findIt = _textures.find(name);

	if (findIt != _textures.end())
		return findIt->second;

	wstring path = _path + name;

	CreateTexture(name, path);

	return _textures[name];
}

const shared_ptr<Mesh>& ResourceManager::GetMesh(size_t name)
{
	return _meshs[name];
}

const std::shared_ptr<Mesh>& ResourceManager::GetParticleMesh(int i)
{
	return _particle[i];
}

void ResourceManager::AddMesh(size_t name, shared_ptr<Mesh> mesh)
{
	_meshs.insert(make_pair(name, mesh));
}

void ResourceManager::AddParticle( shared_ptr<Mesh> mesh)
{
	_particle.push_back(mesh);
}

void ResourceManager::ReSetParticle(std::vector<int>& deleteindex)
{
	//// 삭제된 매쉬을 지워준다. 
	//for (int i = 0; i < deleteindex.size(); i++)
	//{
	//	_particle.erase(_particle.begin() + deleteindex[i]);
	//}
	_particle.clear();
}
