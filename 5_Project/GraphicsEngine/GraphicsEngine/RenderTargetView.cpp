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

	// GetBuffer �޼��带 ȣ���ؼ� SwapChain�� ����Ű�� �����͸� ��´�. ù �Ű������� �ĸ� ������ �ε����̴�.
	// �ĸ���۰� �ϳ��̹Ƿ� 0�� �����Ѵ�. �ι�°�� ������ �������̽� ������ ���� ������� �ĸ� ���۸� ����Ű�� �����͸� �����ش�.
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	// �ĸ���ۿ� ���� ���� Ÿ�ٺ並 ����
	device->CreateRenderTargetView(backBuffer.Get(), 0, _renderTargetView.GetAddressOf());
	backBuffer.Reset();			// GetBuffer�� ȣ���ϸ� �ĸ� ���ۿ����� COM ����Ƚ���� �����ϱ� ������ �� ����ϰ� ���� Reset ���ش�.

	// ���� ���۴� �׳� ���� ������ ��� 2���� �ؽ�ó�̴�.
	// D3D11_TEXTURE2D_DESC ����ü�� ä��� CreateTexture2D �޼��带 ȣ���ؾ��Ѵ�.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;								// �Ӹ� ������ ����. ���� ���ٽ� ���۸� ���� �ؽ��Ĵ� �ϳ��� ������ ��
	depthBufferDesc.ArraySize = 1;								// �ؽ�ó �迭�� �ؽ�ó ����. �굵 �ϳ��� �ʿ�
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		// �ؼ� ����

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

	// D3D�� depth buffer�� depth stencil texture�� ����Ѵٴ� ����� �˵��� ���ٽ� description�� �ۼ��Ѵ�.
	// ����/���ٽǺ� desc �ʱ�ȭ
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// depth stencil view description �ۼ� 
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ����/���ٽǺ� ����
	device->CreateDepthStencilView(_depthStencilBuffer.Get(), 0, _depthStencilView.GetAddressOf());

	// ����Ÿ�ٺ�, ����/���ĽǺ並 ���������ο� ���ε��Ѵ�.
	deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

	// Set the viewport transform.
	/// ����Ʈ ��ȯ�� �����Ѵ�.
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

	// RTT ��ũ������ �ʱ�ȭ�մϴ�. 
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// RTT ��ũ������ �����մϴ�. 
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

	// RTT�� �����մϴ�.
	result = device->CreateTexture2D(&textureDesc, NULL, _renderTarget.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// ���� Ÿ�� �信 ���� ��ũ������ �����մϴ�.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// ���� Ÿ�� �並 �����մϴ�. 
	result = device->CreateRenderTargetView(_renderTarget.Get(), &renderTargetViewDesc, _renderTargetView.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// ���̴� ���ҽ� �信 ���� ��ũ������ �����մϴ�.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// ���̴� ���ҽ� �並 �����մϴ�. 
	result = device->CreateShaderResourceView(_renderTarget.Get(), &shaderResourceViewDesc, _shaderResourceView.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// ���� ���� �����
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

	// Depth Buffer ����
	result = device->CreateTexture2D(&_depthStencilDesc, nullptr, _depthStencilBuffer.GetAddressOf());

	if (FAILED(result))
		return false;

	// D3D�� depth buffer�� depth stencil texture�� ����Ѵٴ� ����� �˵��� ���ٽ� description�� �ۼ��Ѵ�. 
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
	/// ����Ʈ ��ȯ�� �����Ѵ�.
	ZeroMemory(&_screenViewport, sizeof(D3D11_VIEWPORT));
	_screenViewport.TopLeftX = 0;
	_screenViewport.TopLeftY = 0;
	_screenViewport.Width = static_cast<float>(width);
	_screenViewport.Height = static_cast<float>(height);
	_screenViewport.MinDepth = 0.0f;
	_screenViewport.MaxDepth = 1.0f;

	return true;
}


/// ���� ť��� ���ε��� ���� �Լ�
bool RenderTargetView::CubeMapRenderTargetTextureInit(ComPtr<ID3D11Device> device, DXGI_FORMAT format)
{
	// ���� ť�� �ۼ� �� �����ϱ� ���� ���� ���� Ÿ�� ��, ���� ����, ����Ʈ
	// ���� �̰� �ʿ����� �𸣰ڴ�. �ϴ� ������ ���� ������ �ڵ带 �ּ� ó���Ѵ�. 
	//this->_renderTarget = mRenderTargetView;
	//this->_renderTargetView = mDepthStencilView;
	//this->_screenViewport = mScreenViewport;

	// ���� ť�� ���ο����� ä���(fill rate)�� ���̱� ����
	// 256 X 256 ���� ���ػ� ť�� ���� �����
	int m_CubeMapSize = 256;

	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// RTT ��ũ������ �ʱ�ȭ�մϴ�. 
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// RTT ��ũ������ �����մϴ�. 
	textureDesc.Width = m_CubeMapSize;
	textureDesc.Height = m_CubeMapSize;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 6;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Format = format;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	// MiscFlags ����
	// D3D11_RESOURCE_MISC_GENERATE_MIPS : GenerateMips �Լ��� ����� �۵��ϱ� ���� �÷���
	// D3D11_RESOURCE_MISC_TEXTURECUBE : Direct3D�� �ؽ�ó �迭�� �ϳ��� ť�� ������ �����
	// (������ �ؽ�ó�� �ֻ��� �Ӹ� ���ؿ��� ��������)
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

	ComPtr<ID3D11Texture2D> cubeTex = nullptr;

	// RTT�� �����մϴ�.
	result = device->CreateTexture2D(&textureDesc, 0, cubeTex.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	// ���� Ÿ�� �信 ���� ��ũ������ �����մϴ�.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	renderTargetViewDesc.Texture2DArray.ArraySize = 1;	// ���� Ÿ�� �信�� ����� �迭�� �ؽ�ó ��
	renderTargetViewDesc.Texture2DArray.MipSlice = 0;	// �� �����̽��� ����� �Ӹ� ������ �ε���

	for (int i = 0; i < 6; ++i)
	{
		// �ؽ�ó �迭���� ����� ù ���� �ؽ�ó�� �ε���
		renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
		// �迭�� �� ���ҿ� ���� �並 ����
		result = device->CreateRenderTargetView(cubeTex.Get(), &renderTargetViewDesc, _waterRenderTargetView[i].GetAddressOf());
		if (FAILED(result))
		{
			return false;
		}
	}

	// ���������� �ݻ� ȿ���� ���� �ȼ� ���̴����� ť�� ���� ��ü�� ��������
	// ť�� �ʿ� ���� ���̴� ���ҽ� �並 �����ؾ� �Ѵ�
	// ���̴� ���ҽ� �信 ���� ��ũ������ �����մϴ�.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
	shaderResourceViewDesc.TextureCube.MipLevels = -1;

	// ���̴� ���ҽ� �並 �����մϴ�. 
	result = device->CreateShaderResourceView(cubeTex.Get(), &shaderResourceViewDesc, _waterShaderResourceView.GetAddressOf());

	if (FAILED(result))
	{
		return false;
	}

	//cubeTex->Release();

	// ť�� �� ����� �ػ󵵴� �ĸ� ���ۿʹ� �ٸ��� ������
	// ť�� �ʿ� ���� �������Ϸ��� ť�� �� ���� �ػ󵵿� �´� ũ���� ���� ���۰� �ʿ��ϴ�
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
	// Depth Buffer ����
	result = device->CreateTexture2D(&_depthStencilDesc, nullptr, &depthTex);

	if (FAILED(result))
		return false;

	// ť�� ��ü�� ���� ���� ���ٽ� �並 �����Ѵ�
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
	/// ����Ʈ ��ȯ�� �����Ѵ�.
	ZeroMemory(&_cubeMapViewport, sizeof(D3D11_VIEWPORT));
	_cubeMapViewport.TopLeftX = 0;
	_cubeMapViewport.TopLeftY = 0;
	_cubeMapViewport.Width = static_cast<float>(m_CubeMapSize);
	_cubeMapViewport.Height = static_cast<float>(m_CubeMapSize);
	_cubeMapViewport.MinDepth = 0.0f;
	_cubeMapViewport.MaxDepth = 1.0f;


	// �⺻ �������� �־��ش�. 
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
