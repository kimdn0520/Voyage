#include "pch.h"
#include "LightManager.h"

shared_ptr<LightManager> LightManager::lightManager = nullptr;
map<size_t, LightInfo> LightManager::dirLights;
map<size_t, LightInfo> LightManager::pointLights;
map<size_t, LightInfo> LightManager::spotLights;

cbLight LightManager::cbLightBuffer;

shared_ptr<LightManager> LightManager::GetInstance()
{
	if (lightManager == nullptr)
		lightManager = make_shared<LightManager>();

	return lightManager;
}

void LightManager::SetLightInfo(LightInfo lightInfo)
{
	switch (lightInfo.lightType)
	{
	case (int)LIGHT_TYPE::DIRECTIONAL_LIGHT:
	{
		dirLights.insert(make_pair(lightInfo.hashcode, lightInfo));
	}
	break;
	case (int)LIGHT_TYPE::POINT_LIGHT:
	{
		pointLights.insert(make_pair(lightInfo.hashcode, lightInfo));
	}
	break;
	case (int)LIGHT_TYPE::SPOT_LIGHT:
	{
		spotLights.insert(make_pair(lightInfo.hashcode, lightInfo));
	}
	break;
	default:
		break;
	}
}

void LightManager::UpdateLightInfo(LightInfo lightInfo)
{
	switch (lightInfo.lightType)
	{
	case (int)LIGHT_TYPE::DIRECTIONAL_LIGHT:
	{
		dirLights[lightInfo.hashcode].direction = lightInfo.direction;
	}
	break;
	case (int)LIGHT_TYPE::POINT_LIGHT:
	{
		pointLights[lightInfo.hashcode].position = lightInfo.position;
	}
	break;
	case (int)LIGHT_TYPE::SPOT_LIGHT:
	{
		spotLights[lightInfo.hashcode].direction = lightInfo.direction;
		spotLights[lightInfo.hashcode].position = lightInfo.position;
	}
	break;
	default:
		break;
	}
}


void LightManager::ResetLightInfo()
{
	dirLights.clear();
	pointLights.clear();
	spotLights.clear();
}

void LightManager::Init()
{
	
}

void LightManager::Update()
{
	cbLightBuffer.gDirLightCnt = LightManager::dirLights.size();
	cbLightBuffer.gPointLightCnt = LightManager::pointLights.size();
	cbLightBuffer.gSpotLightCnt = LightManager::spotLights.size();
	int i = 0;
	for (auto it = LightManager::dirLights.begin(); it != LightManager::dirLights.end(); ++it)
	{
		cbLightBuffer.gDirLight[i].Ambient = it->second.ambient;
		cbLightBuffer.gDirLight[i].Diffuse = it->second.diffuse;
		cbLightBuffer.gDirLight[i].Specular = it->second.specular;
		cbLightBuffer.gDirLight[i].Direction = it->second.direction;
	}
	i = 0;
	for (auto it = LightManager::pointLights.begin(); it != LightManager::pointLights.end(); ++it, i++)
	{
		cbLightBuffer.gPointLight[i].Ambient = it->second.ambient;
		cbLightBuffer.gPointLight[i].Diffuse = it->second.diffuse;
		cbLightBuffer.gPointLight[i].Specular = it->second.specular;
		cbLightBuffer.gPointLight[i].Position = it->second.position;
		cbLightBuffer.gPointLight[i].Att = it->second.att;
		cbLightBuffer.gPointLight[i].Range = it->second.range;
	}
	i = 0;
	for (auto it = LightManager::spotLights.begin(); it != LightManager::spotLights.end(); ++it, i++)
	{
		cbLightBuffer.gSpotLight[i].Ambient = it->second.ambient;
		cbLightBuffer.gSpotLight[i].Diffuse = it->second.diffuse;
		cbLightBuffer.gSpotLight[i].Specular = it->second.specular;
		cbLightBuffer.gSpotLight[i].Position = it->second.position;
		cbLightBuffer.gSpotLight[i].Direction = it->second.direction;
		cbLightBuffer.gSpotLight[i].Att = it->second.att;
		cbLightBuffer.gSpotLight[i].Range = it->second.range;
		cbLightBuffer.gSpotLight[i].Spot = it->second.spot;
	}
}
