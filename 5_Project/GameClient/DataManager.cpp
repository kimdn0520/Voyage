#include "pch.h"
#include "DataManager.h"
#include "ObjectInfo.h"
#include "GraphicsEngineManager.h"
#include "Resources.h"
#include "MeshData.h"

shared_ptr<DataManager> DataManager::dataManager = nullptr;

std::vector<std::string> DataManager::_pBankName;

shared_ptr<DataManager> DataManager::GetInstance()
{
	if (dataManager == nullptr)
		dataManager = make_shared<DataManager>();

	return dataManager;
}

void DataManager::DataLoad()
{
	AddMaterial();
	
	isDataLoad = true;

	_dataResources.insert(make_pair("AdultBoy", Resources::GetInstance()->LoadASE("Data/ASE/boy.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("SmallBoat", Resources::GetInstance()->LoadASE("Data/ASE/exBoat7.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("AdultGirl", Resources::GetInstance()->LoadASE("Data/ASE/girl.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("BigStar", Resources::GetInstance()->LoadASE("Data/ASE/bigstar.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Clock", Resources::GetInstance()->LoadASE("Data/ASE/clock.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("LilyFlower", Resources::GetInstance()->LoadASE("Data/ASE/lilyflower.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Money", Resources::GetInstance()->LoadASE("Data/ASE/money.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Heart", Resources::GetInstance()->LoadASE("Data/ASE/heart.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Star", Resources::GetInstance()->LoadASE("Data/ASE/star.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Bottle", Resources::GetInstance()->LoadASE("Data/ASE/bottle.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Rose", Resources::GetInstance()->LoadASE("Data/ASE/rose.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("MusicNote", Resources::GetInstance()->LoadASE("Data/ASE/note.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Glacier", Resources::GetInstance()->LoadASE("Data/ASE/glacier.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Boat", Resources::GetInstance()->LoadASE("Data/ASE/boat.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("BigBoat", Resources::GetInstance()->LoadASE("Data/ASE/bigboat.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Island", Resources::GetInstance()->LoadASE("Data/ASE/island.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Island2", Resources::GetInstance()->LoadASE("Data/ASE/island2.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Turtle", Resources::GetInstance()->LoadASE("Data/ASE/turtle.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Bird", Resources::GetInstance()->LoadASE("Data/ASE/bird.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("BirdDie", Resources::GetInstance()->LoadASE("Data/ASE/birddie.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("DodoYouth", Resources::GetInstance()->LoadASE("Data/ASE/dodoYouth.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Lightning", Resources::GetInstance()->LoadASE("Data/ASE/lightning.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Swirl", Resources::GetInstance()->LoadASE("Data/ASE/Swirl.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Baby", Resources::GetInstance()->LoadASE("Data/ASE/baby.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Grandma", Resources::GetInstance()->LoadASE("Data/ASE/grandma.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));
	_dataResources.insert(make_pair("Grandfa", Resources::GetInstance()->LoadASE("Data/ASE/grandfa.ase", (int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID)));

	// Renewal() 도 해줘야함
	Resources::GetInstance()->Renewal();
}

vector<shared_ptr<GameObject>> DataManager::GetDataResources(string data)
{
	return _dataResources[data]->Instantiate();
}

void DataManager::AddMaterial()
{
	///////////////////////////////////////////////////////////////////
	shared_ptr<MaterialInfo> materialInfo3 = make_shared<MaterialInfo>();
	materialInfo3->isUtil = true;

	_defaultID = Resources::GetInstance()->GetMaterialID();

	Resources::GetInstance()->SetMaterial(materialInfo3);
	Resources::GetInstance()->PlusMaterialID();
	///////////////////////////////////////////////////////////////////
	shared_ptr<MaterialInfo> materialInfo = make_shared<MaterialInfo>();
	materialInfo->ambient = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	materialInfo->diffuse = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	materialInfo->specular = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	materialInfo->isDiffuse = false;
	materialInfo->isNormal = false;
	materialInfo->isSpecular = false;
	materialInfo->isUtil = true;

	_skyBoxMaterialID = Resources::GetInstance()->GetMaterialID();	// sky를 만들때 material을 하나 더 만들어서 pushback 해야함

	Resources::GetInstance()->SetMaterial(materialInfo);
	Resources::GetInstance()->PlusMaterialID();
	///////////////////////////////////////////////////////////////////
	shared_ptr<MaterialInfo> materialInfo2 = make_shared<MaterialInfo>();
	materialInfo2->ambient = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	materialInfo2->diffuse = Vector4(0.7f, 0.7f, 0.7f, 1.0f);
	materialInfo2->specular = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	materialInfo2->reflection = Vector4(0.7f, 0.7f, 0.7f, 1.0f);
	materialInfo2->isDiffuse = true;
	materialInfo2->isNormal = true;
	materialInfo2->isSpecular = false;
	materialInfo2->isReflection = true;
	materialInfo2->isUtil = false;

	_diffuseTextureObjID = Resources::GetInstance()->GetMaterialID();	// diffuse 텍스쳐를 붙이고 라이트를 받는 머터리얼 (수동 용)

	Resources::GetInstance()->SetMaterial(materialInfo2);
	Resources::GetInstance()->PlusMaterialID();
	///////////////////////////////////////////////////////////////////
	shared_ptr<MaterialInfo> materialInfo4 = make_shared<MaterialInfo>();
	materialInfo4->ambient = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
	materialInfo4->diffuse = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
	materialInfo4->specular = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
	materialInfo4->reflection = Vector4(0.7f, 0.7f, 0.7f, 1.0f);
	materialInfo4->isDiffuse = false;
	materialInfo4->isNormal = false;
	materialInfo4->isSpecular = false;
	materialInfo4->isReflection = true;
	materialInfo4->isUtil = false;

	_oceanID = Resources::GetInstance()->GetMaterialID();	

	Resources::GetInstance()->SetMaterial(materialInfo4);
	Resources::GetInstance()->PlusMaterialID();

	///////////////////////////////////////////////////////////////////
	shared_ptr<MaterialInfo> materialInfo5 = make_shared<MaterialInfo>();
	materialInfo5->ambient = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
	materialInfo5->diffuse = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
	materialInfo5->specular = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
	materialInfo5->reflection = Vector4(0.7f, 0.7f, 0.7f, 1.0f);
	materialInfo5->isDiffuse = true;
	materialInfo5->isNormal = false;
	materialInfo5->isSpecular = false;
	materialInfo5->isReflection = false;
	materialInfo5->isUtil = false;

	_particleID = Resources::GetInstance()->GetMaterialID();

	Resources::GetInstance()->SetMaterial(materialInfo5);
	Resources::GetInstance()->PlusMaterialID();

	///////////////////////////////////////////////////////////////////
	shared_ptr<MaterialInfo> materialInfo6 = make_shared<MaterialInfo>();
	materialInfo6->ambient = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
	materialInfo6->diffuse = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
	materialInfo6->specular = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
	materialInfo6->reflection = Vector4(0.7f, 0.7f, 0.7f, 1.0f);
	materialInfo6->isDiffuse = true;
	materialInfo6->isNormal = true;
	materialInfo6->isSpecular = false;
	materialInfo6->isReflection = false;
	materialInfo6->isUtil = true;

	_waveID = Resources::GetInstance()->GetMaterialID();

	Resources::GetInstance()->SetMaterial(materialInfo6);
	Resources::GetInstance()->PlusMaterialID();
}

void DataManager::Release()
{
	_dataResources.clear();
}

