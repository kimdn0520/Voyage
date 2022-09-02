#pragma once
#include "ShaderBase.h"

/// <summary>
/// 셰이더를 만들고 관리해주는 매니저!
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

	// cbuffer의 구조체에대한 usage를 SetUsage함수에서 설정해서 넣어주자
	unordered_map<string, D3D11_USAGE> _cbufferUsageList;

	map<string, int> _samplerList;

	wstring _filepath;

public:
	// 최초로 해야할 거.. filePath 쉐이더가 들어있는 디렉터리 경로 설정
	void SetFilePath(const wstring& filePath) { _filepath = filePath; }

	// 그 다음 모든 쉐이더를 만들어준다.
	void CreateAllShaders();

	void Release();

	void SetInfo();

	void AddConstantBuffer();

	void AddSamplerBuffer();

	shared_ptr<ShaderBase> CreateShader(SHADER_TYPE shaderType, const wstring& shaderName, const wstring& path, string entryPoint, const wstring& saveName, const D3D_SHADER_MACRO* macro);

	shared_ptr<ShaderBase> GetShader(wstring name);
};

