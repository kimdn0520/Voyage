#include "pch.h"
#include "SwapChain.h"
#include "Device.h"
#include "GraphicsEngine.h"

SwapChain::SwapChain()
	: _swapChain(nullptr)
{}

SwapChain::~SwapChain()
{}

void SwapChain::Init(shared_ptr<Device> _device)
{
	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = GraphicsEngine::windowInfo.width;
	swapChainDesc.BufferDesc.Height = GraphicsEngine::windowInfo.height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4X MSAA�� ����ϴ°�? 
	if (GraphicsEngine::_enable4xMsaa)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = _device->Get4xMsaaQuality() - 1;
	}
	// No MSAA
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = GraphicsEngine::windowInfo.hwnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Set to full screen or windowed mode. 
	if (FULL_SCREEN)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// IDXGIFactory�� ������ ���� ������,,
	// D3����̽��κ��� dxgi����̽��� ���´�.
	IDXGIDevice* dxgiDevice = 0;
	_device->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	// dxgi����̽��κ��� dxgi����͸� ���´�.
	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	// dxgi����ͷκ��� dxgi���丮�� ���´�.
	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	// dxgiFactory �ν��Ͻ��� ���ؼ� CreateSwapChain �Լ��� ȣ���Ͽ� ����ü���� �����Ѵ�.
	dxgiFactory->CreateSwapChain(_device->GetDevice().Get(), &swapChainDesc, _swapChain.GetAddressOf());

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();
}

void SwapChain::Release()
{
	_swapChain.ReleaseAndGetAddressOf();
}
