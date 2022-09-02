#pragma once
#include "PassBase.h"

class VertexShader;
class PixelShader;
class RenderTargetView;

class BloomPass : public PassBase
{
public:
	BloomPass();
	~BloomPass();

private:
	
	shared_ptr<PixelShader> _DownSampling_PS;

	shared_ptr<RenderTargetView> _mainRenderTargetView;	// Post Processing �ϱ��� ���� renderTarget
	ComPtr<ID3D11ShaderResourceView> _mainSRV;			// Post Processing �ϱ��� ���� �ؽ���

	ComPtr<ID3D11ShaderResourceView> _objectSRV;					// Bloom�� �ϱ� ���� ������Ʈ �ؽ���
	std::vector<ComPtr<ID3D11ShaderResourceView>> _bloomObjects;	// Bloom�� �ϱ� ���� ������Ʈ �ؽ��ĸ��� _ PrePass���� Bloom�� ������Ʈ�� �־��ش�. 

	// DownSampling�� �ϱ� ���� rendertargetview
	shared_ptr<RenderTargetView> _DownSampleBuffer[3];
	ComPtr<ID3D11RenderTargetView> _DownSampleBufferView[3];


private:
	void SetMainSRV();

public:
	void Init() override;

	void Release();

	void BeginRender();

	void Render(vector<shared_ptr<MeshInfo>> meshs) override;

	void SetBloomSRV(vector<ComPtr<ID3D11ShaderResourceView>> objectssrv) { _bloomObjects = objectssrv; }

	void EndRender();
};

