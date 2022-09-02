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

	shared_ptr<RenderTargetView> _mainRenderTargetView;	// Post Processing 하기전 최종 renderTarget
	ComPtr<ID3D11ShaderResourceView> _mainSRV;			// Post Processing 하기전 최종 텍스쳐

	ComPtr<ID3D11ShaderResourceView> _objectSRV;					// Bloom을 하기 위한 오브젝트 텍스쳐
	std::vector<ComPtr<ID3D11ShaderResourceView>> _bloomObjects;	// Bloom을 하기 위한 오브젝트 텍스쳐모음 _ PrePass에서 Bloom할 오브젝트만 넣어준다. 

	// DownSampling을 하기 위한 rendertargetview
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

