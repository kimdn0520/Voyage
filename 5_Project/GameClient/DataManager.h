#pragma once

class MeshData;
class Data;
class GameObject;

class DataManager
{
public:
	DataManager() {};
	~DataManager() {};

private:
	static shared_ptr<DataManager> dataManager;

public:
	static shared_ptr<DataManager> GetInstance();

	static std::vector<std::string> _pBankName;

private:
	unordered_map<string, std::shared_ptr<MeshData>> _dataResources;

	unsigned int _skyBoxMaterialID;

	unsigned int _diffuseTextureObjID;

	unsigned int _defaultID;

	unsigned int _oceanID;

	unsigned int _particleID;

	unsigned int _waveID;

public:
	bool isDataLoad = false;

public:
	void DataLoad();

    std::vector<shared_ptr<GameObject>> GetDataResources(string data); 

	void AddMaterial();

	void Release();

	unsigned int GetSkyBoxId() { return _skyBoxMaterialID; }
	
	unsigned int GetDiffuseTextureObjId() { return _diffuseTextureObjID; }

	unsigned int GetDefaultID() { return _defaultID; }

	unsigned int GetOceanID() { return _oceanID; }

	unsigned int GetParticleID() { return _particleID; }

	unsigned int GetWaveID() { return _waveID; }

};

