#include "pch.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBufferDefine.h"
#include "SamplerStateDefine.h"
#include "ShaderResource.h"

shared_ptr<ShaderManager> ShaderManager::shaderManager = nullptr;

shared_ptr<ShaderManager> ShaderManager::GetInstance()
{
	if (shaderManager == nullptr)
		shaderManager = make_shared<ShaderManager>();

	return shaderManager;
}

void ShaderManager::CreateAllShaders()
{
	D3D_SHADER_MACRO basic_Macro[] = { {"TEXTURE"}, {NULL, NULL} };

	// Ÿ��, ���̴� �����̸�, ���, entrypoint, ���̴� �Ŵ����� ����� �̸�, ���̴� ��ũ��
	CreateShader(SHADER_TYPE::VERTEX, L"Basic_VS.hlsl", _filepath, "Basic_VS", L"Basic_VS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"Basic_PS.hlsl", _filepath, "Basic_PS", L"Basic_PS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"Basic_PS.hlsl", _filepath, "Basic_PS", L"Basic_PS_1", basic_Macro);	 // �ؽ��� ���

	D3D_SHADER_MACRO light_MacroVS1[] = { {"Skinned"}, {NULL, NULL} };										 // ��Ų�� 
	CreateShader(SHADER_TYPE::VERTEX, L"Light_VS.hlsl", _filepath, "Light_VS", L"Light_VS", nullptr);
	CreateShader(SHADER_TYPE::VERTEX, L"Light_VS.hlsl", _filepath, "Light_VS", L"Light_VS_1", light_MacroVS1);
	CreateShader(SHADER_TYPE::PIXEL, L"Light_PS.hlsl", _filepath, "Light_PS", L"Light_PS", nullptr);

	CreateShader(SHADER_TYPE::VERTEX, L"Deferred_VS.hlsl", _filepath, "Deferred_VS", L"Deferred_VS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"Deferred_PS.hlsl", _filepath, "Deferred_PS", L"Deferred_PS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"DeferredMulti_PS.hlsl", _filepath, "DeferredMulti_PS", L"DeferredMulti_PS", nullptr);

	CreateShader(SHADER_TYPE::PIXEL, L"Debug_PS.hlsl", _filepath, "Debug_PS", L"Debug_PS", nullptr);

	CreateShader(SHADER_TYPE::VERTEX, L"SkyBox_VS.hlsl", _filepath, "SkyBox_VS", L"SkyBox_VS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"SkyBox_PS.hlsl", _filepath, "SkyBox_PS", L"SkyBox_PS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"WaterPassSky_PS.hlsl", _filepath, "WaterPassSky_PS", L"WaterPassSky_PS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"WaterPassDefault_PS.hlsl", _filepath, "WaterPassDefault_PS", L"WaterPassDefault_PS", nullptr);

	CreateShader(SHADER_TYPE::PIXEL, L"Normal_PS.hlsl", _filepath, "Normal_PS", L"Normal_PS", nullptr);  

	CreateShader(SHADER_TYPE::VERTEX, L"Water_VS.hlsl", _filepath, "Water_VS", L"Water_VS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"Water_PS.hlsl", _filepath, "Water_PS", L"Water_PS", nullptr);

	CreateShader(SHADER_TYPE::VERTEX, L"UI_VS.hlsl", _filepath, "UI_VS", L"UI_VS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"UI_PS.hlsl", _filepath, "UI_PS", L"UI_PS", nullptr);

	CreateShader(SHADER_TYPE::VERTEX, L"OceanTest_VS.hlsl", _filepath, "OceanTest_VS", L"OceanTest_VS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"OceanTest_PS.hlsl", _filepath, "OceanTest_PS", L"OceanTest_PS", nullptr);

	CreateShader(SHADER_TYPE::PIXEL, L"DownSampling_PS.hlsl", _filepath, "DownSampling_PS", L"DownSampling_PS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"Particle_PS.hlsl", _filepath, "Particle_PS", L"Particle_PS", nullptr);


	CreateShader(SHADER_TYPE::VERTEX, L"WaveEffect_VS.hlsl", _filepath, "WaveEffect_VS", L"WaveEffect_VS", nullptr);
	CreateShader(SHADER_TYPE::PIXEL, L"WaveEffect_PS.hlsl", _filepath, "WaveEffect_PS", L"WaveEffect_PS", nullptr);


	SetInfo();
	AddConstantBuffer();
	AddSamplerBuffer();
}

void ShaderManager::Release()
{
	for (auto& shader : _shaderList)
	{
		shader.second->ReleaseData();
		//shader.second->Release();		// �ϸ� ��������
	}

	_shaderList.clear();
	_cbufferUsageList.clear();
	_samplerList.clear();
}

/// <summary>
/// �߿��� �Լ�!
/// constantBuffer�� �������ְ� sampler�� ��������� �� �� �ִ�!
/// </summary>
void ShaderManager::SetInfo()
{
	_cbufferUsageList.insert(make_pair("cbMesh", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(make_pair("cbLight", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(make_pair("cbMaterial", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(make_pair("cbSkinned", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(make_pair("cbTexture", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(make_pair("cbReflection", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(make_pair("cbWater", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(make_pair("cbUI", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(make_pair("cbOcean", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(make_pair("cbWave", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(make_pair("cbNoise", D3D11_USAGE_DEFAULT));
	_cbufferUsageList.insert(make_pair("cbDistortion", D3D11_USAGE_DEFAULT));

	_samplerList.insert(make_pair("samAnisotropicWrap", (int)SAMSTATE::AnisotropicWrap));
	_samplerList.insert(make_pair("samAnisotropicClamp", (int)SAMSTATE::AnisotropicClamp));
	_samplerList.insert(make_pair("samLinearWrap", (int)SAMSTATE::LinearWrap));
	_samplerList.insert(make_pair("samLinearClamp", (int)SAMSTATE::LinearClamp));
	/*_samplerList.insert(make_pair(hasher("samPointWrap"), (int)SAMSTATE::PointWrap));
	_samplerList.insert(make_pair(hasher("samPointClamp"), (int)SAMSTATE::PointClamp));*/
}

void ShaderManager::AddConstantBuffer()
{
	// ���̴��� �ִ� cbuffer ���� usage ���� �� ���� ����
	for (auto& shader : _shaderList)
	{
		for (auto& cBuffer : shader.second->GetConstantBufferData())
		{
			auto it = _cbufferUsageList.find(cBuffer.first);
			
			// ���� Ű�� ã�Ƽ� usage�� ���´�.
			if (it != _cbufferUsageList.end())
			{
				cBuffer.second->usage = it->second;				// usage �־��ش�
			}
		}

		// usage �� �־������� ���� �������ش�.
		shader.second->SetConstantBuffer();
	}
}

void ShaderManager::AddSamplerBuffer()
{
	for (auto& shader : _shaderList)
	{
		for (auto& samplerState : shader.second->GetSamplerStateData())
		{
			shader.second->SetSamplerBuffer(_samplerList[samplerState.second->name], samplerState.second->register_slot);
		}
	}
}

/// <summary>
/// ���̴��� ������ִ� �Լ�!
/// </summary>
shared_ptr<ShaderBase> ShaderManager::CreateShader(SHADER_TYPE shaderType, const wstring& shaderName, const wstring& path, string entryPoint, const wstring& saveName, const D3D_SHADER_MACRO* macro)
{
	shared_ptr<ShaderBase> newShader;
	
	switch (shaderType)
	{
		case SHADER_TYPE::VERTEX:
			newShader = make_shared<VertexShader>(shaderName, path, entryPoint, macro);
			break;
		case SHADER_TYPE::PIXEL:
			newShader = make_shared<PixelShader>(shaderName, path, entryPoint, macro);
			break;
		case SHADER_TYPE::COMPUTE:
			// newShader = new ComputeShader(shaderName, path);
			break;
		default:
			return nullptr;
	}

	if (newShader == nullptr)
		return nullptr;

	// �����ϸ鼭 ���̴�����Ʈ�� ��Ƶд�.
	_shaderList.insert(make_pair(saveName, newShader));

	return newShader;
}

shared_ptr<ShaderBase> ShaderManager::GetShader(wstring name)
{
	return _shaderList[name];
}
