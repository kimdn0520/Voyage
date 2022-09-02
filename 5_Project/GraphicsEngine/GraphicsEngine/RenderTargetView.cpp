#include "pch.h"
#include "RenderTargetView.h"

RenderTargetView::RenderTargetView()
{
	_renderTarget = nullptr;
	_renderTargetView = nullptr;
	_shaderResourceView = nullptr;
	_waterShaderResourceView = nullptr;
	for (int i = 0; i < 6; ++i)
		_waterRenderTargetView[i] = nullptr;
}

RenderTargetView::~RenderTargetView()
{}

void RenderTargetView::BackBufferRenderTarget(
	const ComPtr<ID3D11Device>& device,
	const ComPtr<ID3D11DeviceContext>& deviceContext,
	const ComPtr<IDXGISwapChain>& swapChain,
	unsigned int MsaaQuality, bool enable4xMsaa, int width, int height)
{
	// Resize the swap chain and recreate the render target view.
	HRESULT hr = swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	if (FAILED(hr))
		MessageBox(0, L"ResizeBuffers Failed.", 0, 0);

	ComPtr<ID3D11Texture2D> backBuffer;

	// GetBuffer 메서드를 호출해서 SwapChain을 가리키는 포인터를 얻는다. 첫 매개변수는 후면 버퍼의 인덱스이다.
	// 후면버퍼가 하나이므로 0을 지정한다. 두번째는 버퍼의 인터페이스 형식을 지정 세번재는 후면 버퍼를 가리키는 포인터를 돌려준다.
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	// 후면버퍼에 대한 렌더 타겟뷰를 생성
	device->CreateRenderTargetView(backBuffer.Get(), 0, _renderTargetView.GetAddressOf());
	backBuffer.Reset();			// GetBuffer를 호출하면 후면 버퍼에대한 COM 참조횟수가 증가하기 때문에 다 사용하고 나면 Reset 해준다.

	// 깊이 버퍼는 그냥 깊이 정보를 담는 2차원 텍스처이다.
	// D3D11_TEXTURE2D_DESC 구조체를 채우고 CreateTexture2D 메서드를 호출해야한다.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;								// 밉맵 수준의 갯수. 깊이 스텐실 버퍼를 위한 텍스쳐는 하나만 있으면 돼
	depthBufferDesc.ArraySize = 1;								// 텍스처 배열의 텍스처 갯수. 얘도 하나만 필요
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		// 텍셀 형식

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (enable4xMsaa)
	{
		depthBufferDesc.SampleDesc.Count = 4;
		depthBufferDesc.SampleDesc.Quality = MsaaQuality - 1;
	}
	// No MSAA
	else
	{
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
	}

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	device->CreateTexture2D(&depthBufferDesc, 0, _depthStencilBuffer.GetAddressOf());

	// D3D가 depth buffer을 depth stencil texture로 사용한다는 사실을 알도록 스텐실 description을 작성한다.
	// 뎁스/스텐실뷰 desc 초기화
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// depth stencil view description 작성 
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 뎁스/스텐실뷰 생성
	device->CreateDepthStencilView(_depthStencilBuffer.Get(), 0, _depthStencilView.GetAddressOf());

	// 렌더타겟뷰, 뎁스/스탠실뷰를 파이프라인에 바인딩한다.
	deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

	// Set the viewport transform.
	/// 뷰포트 변환을 셋팅한다.
	_screenViewport.TopLeftX = 0;
	_screenViewport.TopLeftY = 0;
	_screenViewport.Width = static_cast<float>(width);
	_screenViewport.Height = static_cast<float>(height);
	_screenViewport.MinDepth = 0.0f;
	_screenViewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &_screenViewport);
}

bool RenderTargetView::RenderTargetTextureInit(ComPtr<ID3D11Device> device, int width, int height, DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// RTT 디스크립션을 초기화합니다. 
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// RTT 디스크립션을 세팅합니다. 
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// RTT를 생성합니다.
	result = device->CreateTexture2D(&textureDesc, NULL, _renderTarget.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// 렌더 타겟 뷰에 대한 디스크립션을 설정합니다.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// 렌더 타겟 뷰를 생성합니다. 
	result = device->CreateRenderTargetView(_renderTarget.Get(), &renderTargetViewDesc, _renderTargetView.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// 셰이더 리소스 뷰에 대한 디스크립션을 설정합니다.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// 셰이더 리소스 뷰를 생성합니다. 
	result = device->CreateShaderResourceView(_renderTarget.Get(), &shaderResourceViewDesc, _shaderResourceView.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// 깊이 버퍼 만들기
	D3D11_TEXTURE2D_DESC _depthStencilDesc;
	ZeroMemory(&_depthStencilDesc, sizeof(_depthStencilDesc));

	_depthStencilDesc.Width = width;
	_depthStencilDesc.Height = height;
	_depthStencilDesc.MipLevels = 1;
	_depthStencilDesc.ArraySize = 1;
	_depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	_depthStencilDesc.SampleDesc.Count = 1;
	_depthStencilDesc.SampleDesc.Quality = 0;

	_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	_depthStencilDesc.CPUAccessFlags = 0;
	_depthStencilDesc.MiscFlags = 0;

	// Depth Buffer 생성
	result = device->CreateTexture2D(&_depthStencilDesc, nullptr, _depthStencilBuffer.GetAddressOf());

	if (FAILED(result))
		return false;

	// D3D가 depth buffer을 depth stencil texture로 사용한다는 사실을 알도록 스텐실 description을 작성한다. 
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description. 
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(_depthStencilBuffer.Get(), 0, _depthStencilView.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// Set the viewport transform.
	/// 뷰포트 변환을 셋팅한다.
	ZeroMemory(&_screenViewport, sizeof(D3D11_VIEWPORT));
	_screenViewport.TopLeftX = 0;
	_screenViewport.TopLeftY = 0;
	_screenViewport.Width = static_cast<float>(width);
	_screenViewport.Height = static_cast<float>(height);
	_screenViewport.MinDepth = 0.0f;
	_screenViewport.MaxDepth = 1.0f;

	return true;
}


/// 동적 큐브맵 바인딩을 위한 함수
bool RenderTargetView::CubeMapRenderTargetTextureInit(ComPtr<ID3D11Device> device, DXGI_FORMAT format)
{
	// 동적 큐브 작성 후 복원하기 위한 원본 렌더 타겟 뷰, 깊이 버퍼, 뷰포트
	// 현재 이게 필요한지 모르겠다. 일단 나중을 위해 가져온 코드를 주석 처리한다. 
	//this->_renderTarget = mRenderTargetView;
	//this->_renderTargetView = mDepthStencilView;
	//this->_screenViewport = mScreenViewport;

	// 동적 큐브 매핑에서는 채움률(fill rate)을 줄이기 위해
	// 256 X 256 같은 저해상도 큐브 맵을 사용함
	int m_CubeMapSize = 256;

	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// RTT 디스크립션을 초기화합니다. 
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// RTT 디스크립션을 세팅합니다. 
	textureDesc.Width = m_CubeMapSize;
	textureDesc.Height = m_CubeMapSize;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 6;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Format = format;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	// MiscFlags 설정
	// D3D11_RESOURCE_MISC_GENERATE_MIPS : GenerateMips 함수가 제대로 작동하기 위한 플래그
	// D3D11_RESOURCE_MISC_TEXTURECUBE : Direct3D가 텍스처 배열을 하나의 큐브 맵으로 취급함
	// (없으면 텍스처의 최상위 밉맵 수준에만 렌더링함)
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

	ComPtr<ID3D11Texture2D> cubeTex = nullptr;

	// RTT를 생성합니다.
	result = device->CreateTexture2D(&textureDesc, 0, cubeTex.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// 렌더 타겟 뷰에 대한 디스크립션을 설정합니다.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	renderTargetViewDesc.Texture2DArray.ArraySize = 1;	// 렌터 타겟 뷰에서 사용할 배열의 텍스처 수
	renderTargetViewDesc.Texture2DArray.MipSlice = 0;	// 밉 슬라이스를 사용할 밉맵 수준의 인덱스

	for (int i = 0; i < 6; ++i)
	{
		// 텍스처 배열에서 사용할 첫 번쨰 텍스처의 인덱스
		renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
		// 배열의 한 원소에 대한 뷰를 생성
		result = device->CreateRenderTargetView(cubeTex.Get(), &renderTargetViewDesc, _waterRenderTargetView[i].GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}
	}

	// 마지막으로 반사 효과를 위해 픽셀 셰이더에서 큐브 맵을 물체에 입히려면
	// 큐브 맵에 대한 셰이더 리소스 뷰를 생성해야 한다
	// 셰이더 리소스 뷰에 대한 디스크립션을 설정합니다.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
	shaderResourceViewDesc.TextureCube.MipLevels = -1;

	// 셰이더 리소스 뷰를 생성합니다. 
	result = device->CreateShaderResourceView(cubeTex.Get(), &shaderResourceViewDesc, _waterShaderResourceView.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	//cubeTex->Release();

	// 큐브 맵 면들의 해상도는 후면 버퍼와는 다르기 때문에
	// 큐브 맵에 씬을 렌더링하려면 큐브 맵 면의 해상도에 맞는 크기의 깊이 버퍼가 필요하다
	D3D11_TEXTURE2D_DESC _depthStencilDesc;
	ZeroMemory(&_depthStencilDesc, sizeof(_depthStencilDesc));

	_depthStencilDesc.Width = m_CubeMapSize;
	_depthStencilDesc.Height = m_CubeMapSize;
	_depthStencilDesc.MipLevels = 1;
	_depthStencilDesc.ArraySize = 1;
	_depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;

	_depthStencilDesc.SampleDesc.Count = 1;
	_depthStencilDesc.SampleDesc.Quality = 0;

	_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	_depthStencilDesc.CPUAccessFlags = 0;
	_depthStencilDesc.MiscFlags = 0;

	ID3D11Texture2D* depthTex = 0;
	// Depth Buffer 생성
	result = device->CreateTexture2D(&_depthStencilDesc, nullptr, &depthTex);

	if (FAILED(result))
		return false;

	// 큐브 전체에 대한 깊이 스텐실 뷰를 생성한다
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description. 
	depthStencilViewDesc.Format = _depthStencilDesc.Format;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(depthTex, &depthStencilViewDesc, _waterDepthStencilView.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// Set the viewport transform.
	/// 뷰포트 변환을 셋팅한다.
	ZeroMemory(&_cubeMapViewport, sizeof(D3D11_VIEWPORT));
	_cubeMapViewport.TopLeftX = 0;
	_cubeMapViewport.TopLeftY = 0;
	_cubeMapViewport.Width = static_cast<float>(m_CubeMapSize);
	_cubeMapViewport.Height = static_cast<float>(m_CubeMapSize);
	_cubeMapViewport.MinDepth = 0.0f;
	_cubeMapViewport.MaxDepth = 1.0f;


	// 기본 세팅으로 넣어준다. 
	//this->_renderTarget = _waterRenderTargetView[0];
	//this->_renderTargetView = _waterShaderResourceView;
	//this->_shaderResourceView = _waterShaderResourceView;
	//this->_screenViewport = _cubeMapViewport;

	return true;
}


void RenderTargetView::ClearRenderTarget(ComPtr<ID3D11DeviceContext> deviceContext, Vector4 inputColor)
{
	float color[4];

	// Setup the color to clear the buffer to. 
	color[0] = inputColor.x;
	color[1] = inputColor.y;
	color[2] = inputColor.z;
	color[3] = inputColor.w;

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(_renderTargetView.Get(), color);
	// Clear the depth buffer. 
	deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderTargetView::ClearCubeRenderTarget(int index, ComPtr<ID3D11DeviceContext> deviceContext, Vector4 inputColor)
{
	float color[4];

	// Setup the color to clear the buffer to. 
	color[0] = inputColor.x;
	color[1] = inputColor.y;
	color[2] = inputColor.z;
	color[3] = inputColor.w;

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(_waterRenderTargetView[index].Get(), color);
	// Clear the depth buffer. 
	deviceContext->ClearDepthStencilView(_waterDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderTargetView::Release()
{
	_renderTarget.ReleaseAndGetAddressOf();
	_renderTargetView.ReleaseAndGetAddressOf();
	_shaderResourceView.ReleaseAndGetAddressOf();
	_depthStencilBuffer.ReleaseAndGetAddressOf();
	_depthStencilView.ReleaseAndGetAddressOf();
}

void RenderTargetView::ResizeRelease()
{
	_renderTargetView.ReleaseAndGetAddressOf();
	_depthStencilBuffer.ReleaseAndGetAddressOf();
	_depthStencilView.ReleaseAndGetAddressOf();
}
