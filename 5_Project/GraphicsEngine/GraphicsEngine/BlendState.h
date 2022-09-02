#pragma once

class Device;

class BlendState
{
public:
	BlendState();
	~BlendState();

private:
	ComPtr<ID3D11BlendState> _blendState;

public:
	void Init(shared_ptr<Device> device);

	ComPtr<ID3D11BlendState> GetBlendState() { return _blendState; }

	void Release();
};

