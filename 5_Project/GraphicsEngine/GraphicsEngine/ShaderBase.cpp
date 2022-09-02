#include "pch.h"
#include "ShaderBase.h"
#include "GraphicsEngineAPI.h"
#include "SamplerStateDefine.h"
#include "ResourceManager.h"
#include "Texture.h"

ShaderBase::ShaderBase(SHADER_TYPE shaderType, wstring shaderName)
	: _shaderType(shaderType), _shaderName(shaderName)
{}

ShaderBase::~ShaderBase()
{}

void ShaderBase::ReleaseData()
{
	_constantBufferData.clear();
	_samplerStateData.clear();
	_shaderResourceViewData.clear();
	_constantBuffers.clear();
	_samplerStates.clear();
}

void ShaderBase::SetConstantBuffer()
{
	UINT cBufferFlag = 0;

	for (auto& cBuffer : _constantBufferData)
	{
		// Flag 汲沥
		switch (cBuffer.second->usage)
		{
		case D3D11_USAGE_DEFAULT:
			break;
		case D3D11_USAGE_IMMUTABLE:
			break;
		case D3D11_USAGE_DYNAMIC:
			cBufferFlag = D3D11_CPU_ACCESS_WRITE;
			break;
		case D3D11_USAGE_STAGING:
			cBufferFlag = D3D11_CPU_ACCESS_READ;
			break;
		default:
			break;
		}

		ComPtr<ID3D11Buffer> buffer = nullptr;
		
		// description 累己
		CD3D11_BUFFER_DESC cBufferDesc(
			(UINT)cBuffer.second->size,
			D3D11_BIND_CONSTANT_BUFFER,
			cBuffer.second->usage, cBufferFlag);

		// 滚欺 积己
		GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice()->
			CreateBuffer(&cBufferDesc, nullptr, &buffer);
		
		// 积己茄 滚欺 持绢林扁
		_constantBuffers[cBuffer.second->register_slot] = buffer;
	}
}

void ShaderBase::SetSamplerBuffer(int sampler, int register_slot)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 1;

	switch (sampler)
	{
		case (int)SAMSTATE::AnisotropicWrap:
		{
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.MaxAnisotropy = 4;
		}
		break;
		case (int)SAMSTATE::AnisotropicClamp:
		{
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.MaxAnisotropy = 4;
		}
		break;
		case (int)SAMSTATE::LinearWrap:
		{
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		}
		break;
		case (int)SAMSTATE::LinearClamp:
		{
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		}
		break;
	}
		
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ComPtr<ID3D11SamplerState> sampleState;

	// sampler 积己
	GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice()->
		CreateSamplerState(&samplerDesc, &sampleState);

	// 积己茄 sampler 持绢林扁
	_samplerStates[register_slot] = sampleState;
}

void ShaderBase::SetResourceViewBuffer(wstring texName, string texture)
{
	int register_slot = _shaderResourceViewData[texture]->register_slot;

	_shaderResourceViews[register_slot] = ResourceManager::GetInstance()->GetTexture(texName)->GetMapSRV();
}

void ShaderBase::SetResourceViewBuffer(ComPtr<ID3D11ShaderResourceView> srv, string texture)
{
	int register_slot = _shaderResourceViewData[texture]->register_slot;

	_shaderResourceViews[register_slot] = srv;
}
