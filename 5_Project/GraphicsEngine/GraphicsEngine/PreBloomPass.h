#pragma once
#include "PassBase.h"

class VertexShader;
class PixelShader;
class RenderTargetView;

class PreBloomPass : public PassBase
{
public:
	PreBloomPass();
	~PreBloomPass();

private:
	shared_ptr<VertexShader> _deferred_VS;
	shared_ptr<PixelShader> _waterPassDefault;

	shared_ptr<VertexShader> _light_VS;
	shared_ptr<VertexShader> _light_VS_1;

	shared_ptr<VertexShader> _skyBox_VS;
	shared_ptr<PixelShader> _skyBox_PS;

	// Bloom 을 하기 위한 변수
	shared_ptr<RenderTargetView> _gBuffer;
	ComPtr<ID3D11RenderTargetView> _gBufferView;

	vector< ComPtr<ID3D11ShaderResourceView>> _gObjectsSRV;

public:
	void Init() override;

	void Release();

	void Render(vector<shared_ptr<MeshInfo>> meshs) override;

	void BeginRender();

	// Bloom 할 Object를 그린 텍스쳐를 넘겨준다. 
	vector<ComPtr<ID3D11ShaderResourceView>> GetBloomObjectsSRV();

};

