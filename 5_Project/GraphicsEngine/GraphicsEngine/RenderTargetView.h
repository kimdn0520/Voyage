#pragma once
#include <vector>

class RenderTargetView
{
public:
	RenderTargetView();
	~RenderTargetView();

private:
	ComPtr<ID3D11Texture2D> _renderTarget;					    // 그려줄 공간
	ComPtr<ID3D11RenderTargetView> _renderTargetView;			// 렌더타겟 뷰
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;		// 셰이더 리소스 뷰

	ComPtr<ID3D11Texture2D> _depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
	D3D11_VIEWPORT _screenViewport;

	// water를 위한 데이터 
	ComPtr<ID3D11RenderTargetView> _waterRenderTargetView[6];			 
	ComPtr<ID3D11ShaderResourceView> _waterShaderResourceView;			 
	ComPtr<ID3D11DepthStencilView> _waterDepthStencilView;		
	D3D11_VIEWPORT _cubeMapViewport;

public:
	void BackBufferRenderTarget(
		const ComPtr<ID3D11Device>& device, 
		const ComPtr<ID3D11DeviceContext>& deviceContext,
		const ComPtr<IDXGISwapChain>& swapChain,
		unsigned int MsaaQuality,
		bool enable4xMsaa,
		int width, int height);

	bool RenderTargetTextureInit(ComPtr<ID3D11Device> device, int width, int height, DXGI_FORMAT format);

	ComPtr<ID3D11RenderTargetView> GetRenderTargetView() { return _renderTargetView; }
	ComPtr<ID3D11ShaderResourceView> GetSRV() { return _shaderResourceView; }
	ComPtr<ID3D11DepthStencilView> GetDepthStencilView() { return _depthStencilView; }
	D3D11_VIEWPORT& GetViewPort() { return _screenViewport; }

	bool CubeMapRenderTargetTextureInit(ComPtr<ID3D11Device> device/*, ID3D11RenderTargetView* mRenderTargetView, ID3D11DepthStencilView* mDepthStencilView, D3D11_VIEWPORT mScreenViewportM*/, DXGI_FORMAT format);

	void SetRenderTargetView(int i) { _renderTargetView = _waterRenderTargetView[i]; }

	void ClearRenderTarget(ComPtr<ID3D11DeviceContext> deviceContext, Vector4 inputColor);
	void ClearCubeRenderTarget(int index, ComPtr<ID3D11DeviceContext> deviceContext, Vector4 inputColor);

	void Release();
	void ResizeRelease();

	// CubeMap을 위한 Getter
	ComPtr<ID3D11RenderTargetView> GetCubeRenderTargetView(int index) { return _waterRenderTargetView[index]; }
	ComPtr<ID3D11ShaderResourceView> GetCubeSRV() { return _waterShaderResourceView; }
	ComPtr<ID3D11DepthStencilView> GetCubeDepthStencilView() { return _waterDepthStencilView; }
	D3D11_VIEWPORT& GetCubeViewPort() { return _cubeMapViewport; }




};

