#pragma once

class Device;

class RasterizerState
{
public:
	RasterizerState();
	~RasterizerState();

private:
	ComPtr<ID3D11RasterizerState> _rasterizerState;

public:
	ComPtr<ID3D11RasterizerState> GetRasterizerState() { return _rasterizerState; }

	void Init(shared_ptr<Device> device, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode);

	void Release();
};

