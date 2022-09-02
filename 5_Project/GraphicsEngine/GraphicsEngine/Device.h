#pragma once

class Device
{
public:
	Device();
	~Device();

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;

	unsigned int _4xMsaaQuality;

public:
	// device 와 deviceContext를 생성한다.
	void Init();
	void Release();

	const ComPtr<ID3D11Device>& GetDevice() { return _device; }
	const ComPtr<ID3D11DeviceContext>& GetDeviceContext() { return _deviceContext; }
	const unsigned int& Get4xMsaaQuality() { return _4xMsaaQuality; }
};

