#pragma once
#include "ShaderBase.h"

/// <summary>
/// ���̴��� ����� �������ִ� �Ŵ���!
/// </summary>
class ShaderManager
{
public:
	ShaderManager() {};
	~ShaderManager() {};

private:
	static shared_ptr<ShaderManager> shaderManager;

public:
	static shared_ptr<ShaderManager> GetInstance();

private:
	unordered_map<wstring, shared_ptr<ShaderBase>> _shaderList;

	// cbuffer�� ����ü������ usage�� SetUsage�Լ����� �����ؼ� �־�����
	unordered_map<string, D3D11_USAGE> _cbufferUsageList;

	map<string, int> _samplerList;

	wstring _filepath;

public:
	// ���ʷ� �ؾ��� ��.. filePath ���̴��� ����ִ� ���͸� ��� ����
	void SetFilePath(const wstring& filePath) { _filepath = filePath; }

	// �� ���� ��� ���̴��� ������ش�.
	void CreateAllShaders();

	void Release();

	void SetInfo();

	void AddConstantBuffer();

	void AddSamplerBuffer();

	shared_ptr<ShaderBase> CreateShader(SHADER_TYPE shaderType, const wstring& shaderName, const wstring& path, string entryPoint, const wstring& saveName, const D3D_SHADER_MACRO* macro);

	shared_ptr<ShaderBase> GetShader(wstring name);
};

