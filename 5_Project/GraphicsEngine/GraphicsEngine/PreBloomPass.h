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

	// Bloom �� �ϱ� ���� ����
	shared_ptr<RenderTargetView> _gBuffer;
	ComPtr<ID3D11RenderTargetView> _gBufferView;

	vector< ComPtr<ID3D11ShaderResourceView>> _gObjectsSRV;

public:
	void Init() override;

	void Release();

	void Render(vector<shared_ptr<MeshInfo>> meshs) override;

	void BeginRender();

	// Bloom �� Object�� �׸� �ؽ��ĸ� �Ѱ��ش�. 
	vector<ComPtr<ID3D11ShaderResourceView>> GetBloomObjectsSRV();

};

