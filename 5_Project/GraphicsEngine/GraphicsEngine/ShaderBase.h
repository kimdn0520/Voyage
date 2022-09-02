#pragma once
#include "GraphicsEngine.h"
#include "ShaderResource.h"
#include "Device.h"

enum class SHADER_TYPE
{
	VERTEX,
	PIXEL,
	COMPUTE,
};

class ShaderBase
{
public:
	ShaderBase(SHADER_TYPE shaderType, wstring shaderName);
	~ShaderBase();

private:
	SHADER_TYPE _shaderType;
	wstring _shaderName;

protected:
	unordered_map<string, shared_ptr<ConstantBuffer>> _constantBufferData;
	unordered_map<string, shared_ptr<SamplerBuffer>> _samplerStateData;
	unordered_map<string, shared_ptr<ShaderResourceBuffer>> _shaderResourceViewData;

	vector<ComPtr<ID3D11Buffer>> _constantBuffers;
	vector<ComPtr<ID3D11SamplerState>> _samplerStates;
	vector<ComPtr<ID3D11ShaderResourceView>> _shaderResourceViews;
	
public:
	unordered_map<string, shared_ptr<ConstantBuffer>>& GetConstantBufferData() { return _constantBufferData; }
	unordered_map<string, shared_ptr<SamplerBuffer>>& GetSamplerStateData() { return _samplerStateData; }
	
	void ReleaseData();

	void SetConstantBuffer();
	void SetSamplerBuffer(int sampler, int register_slot);

	// 오버로딩
	void SetResourceViewBuffer(wstring texName, string texture);
	void SetResourceViewBuffer(ComPtr<ID3D11ShaderResourceView> srv, string texture);

public:
	template <typename T>
	void ConstantBufferUpdate(T* cbuffer, string name);

	virtual void CreateShader(const wstring& path, string entryPoint, const D3D_SHADER_MACRO* macro) abstract;

	virtual void Release() abstract;

	virtual void Update() abstract;
};

template<typename T>
inline void ShaderBase::ConstantBufferUpdate(T* cbuffer, string name)
{
	auto it = _constantBufferData.find(name);

	switch (it->second->usage)
	{
	case D3D11_USAGE_DEFAULT:
	case D3D11_USAGE_IMMUTABLE:
	{
		GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()
			->UpdateSubresource(_constantBuffers[it->second->register_slot].Get(),
								0, nullptr,
								cbuffer, 0, 0);
	}
	break;
	case D3D11_USAGE_DYNAMIC:
	{
		// Mapping SubResource Data..
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		// GPU Access Lock Buffer Data..
		GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()->
			Map(_constantBuffers[it->second->register_slot].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		// Copy Resource Data..
		memcpy(mappedResource.pData, cbuffer, it->second->size);

		// GPU Access UnLock Buffer Data..
		GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()
			->Unmap(_constantBuffers[it->second->register_slot].Get(), 0);
	}
	break;
	case D3D11_USAGE_STAGING:
		break;
	default:
		break;
	}
}
