#pragma once
#include "NewbieEngineDLL.h"
#include "BoundingVolumes.h"

class MeshData;
class GameObject;
class Data;
class ASEParser;
struct AnimationClipInfo;
struct GraphicsVertex;
struct MaterialInfo;
class ParticleInfo;

class Resources
{	
public:
	NewbieEngine_DLL Resources() {};
	~Resources() {};

private:
	static shared_ptr<Resources> resources;

public:
	static NewbieEngine_DLL shared_ptr<Resources> GetInstance();

private:
	shared_ptr<ASEParser> _aseParser;

	unordered_map<string, shared_ptr<MeshData>> aseMeshData;

	unordered_map<size_t, std::shared_ptr<BoundingCube>> _meshBoundingCube;	// 컬링에 필요한 바운딩큐브

	std::vector<std::string> _bankNames;

	std::shared_ptr<ParticleInfo> _particleInfo;

public:
	void Init();

	void CreateMeshBoundingCube(const std::vector<GraphicsVertex>& vertexVector, const size_t& meshName);

	const shared_ptr<BoundingCube>& GetBoundingCube(size_t name) 
	{ 
		return _meshBoundingCube[name]; 
	}
	
	void LoadCubeMesh(int topology, int rasterState);

	void LoadWireGreenCubeMesh(int topology, int rasterState);
	
	void LoadWireRedCubeMesh(int topology, int rasterState);

	void LoadSphereMesh(int topology, int rasterState);

	void LoadWireSphereGreenMesh(int topology, int rasterState);

	void LoadWireSphereRedMesh(int topology, int rasterState);

	void LoadAxisMesh(int topology, int rasterState);

	void LoadGridMesh(int topology, int rasterState);

	void LoadScreenMesh(int topology, int rasterState);

	void LoadCircleMesh(int topology, int rasterState);

	void LoadOceanMesh(int topology, int rasterState);

	void LoadRectMesh(int topology, int rasterState);

	void LoadRectWireMesh(int topology, int rasterState);

	void LoadWaveMesh(int topology, int rasterState);

	NewbieEngine_DLL void LoadParticleMesh(int topology, int rasterState);

	NewbieEngine_DLL shared_ptr<MeshData> LoadFBX(const wstring& path, int topology, int rasterizerState, int pass);

	NewbieEngine_DLL shared_ptr<MeshData> LoadASE(string path, int topology, int rasterizerState);

	NewbieEngine_DLL unsigned int GetMaterialID();

	NewbieEngine_DLL void PlusMaterialID();

	NewbieEngine_DLL void Renewal();

	NewbieEngine_DLL void SetMaterial(shared_ptr<MaterialInfo> materialInfo);
	
	NewbieEngine_DLL void SetCubeMapTexture(int materialID, wstring cubeMapTexture);

	NewbieEngine_DLL void SetDiffuseMapTexture(int materialID, wstring diffuseMapTexture);
	
	NewbieEngine_DLL void SetNormalMapTexture(int materialID, wstring normalMapTexture);

	NewbieEngine_DLL void SetParticleInfo(shared_ptr<ParticleInfo> particleInfo);

	std::vector<std::string>& GetBankNames() { return _bankNames; }

	void InputEntryDir(std::string& path);

	void GetAllDir(std::string& path, std::vector<std::string>& dirs);

	void GetContents(std::string& path, std::vector<std::string>& dirs);

	void StorePath(std::string& path);

	void ReverseString(std::string& str);
};

