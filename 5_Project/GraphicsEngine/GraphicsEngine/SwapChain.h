#pragma once

class Device;

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

private:
	ComPtr<IDXGISwapChain> _swapChain;

public:
	ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }

	void Init(shared_ptr<Device> _device);

	void Release();
};

