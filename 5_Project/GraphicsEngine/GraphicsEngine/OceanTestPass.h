#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;

class OceanTestPass : public PassBase
{
public:
	OceanTestPass();
	~OceanTestPass();

private:
	shared_ptr<VertexShader> _OceanTest_VS;
	shared_ptr<PixelShader> _OceanTest_PS;

	float _waterTranslation = 0.f;
	float _wateruvScale=0.f;

	ComPtr<ID3D11ShaderResourceView> _gReflectionSkySRV;
	ComPtr<ID3D11ShaderResourceView> _gReflectionObjectSRV;

public:
	float _waveTime=0.0f;

	void Init() override;

	void Release();

	void BeginRender();

	void Render(vector<shared_ptr<MeshInfo>> meshs) override;

	void SetReflectionSkySRV(ComPtr<ID3D11ShaderResourceView> reflectionskysrv) { _gReflectionSkySRV = reflectionskysrv; }
	void SetReflectionObjectSRV(ComPtr<ID3D11ShaderResourceView> reflectionobjectsrv) { _gReflectionObjectSRV = reflectionobjectsrv; }


};

