#pragma once
#include "ShaderBase.h"

class ConstantBuffer;

class PixelShader : public ShaderBase
{
public:
	PixelShader(const wstring& shaderName, const wstring& path, string entryPoint, const D3D_SHADER_MACRO* macro);
	~PixelShader();

private:
	ComPtr<ID3D11PixelShader> _ps;

public:
	void CreateShader(const wstring& path, string entryPoint, const D3D_SHADER_MACRO* macro) override;

	void Update() override;

	void Release() override;
};

