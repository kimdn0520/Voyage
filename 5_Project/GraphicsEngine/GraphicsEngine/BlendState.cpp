#include "pch.h"
#include "BlendState.h"
#include "Device.h"

BlendState::BlendState()
{}

BlendState::~BlendState()
{}

void BlendState::Init(shared_ptr<Device> device)
{
	D3D11_BLEND_DESC blendStateDes = {};

	ZeroMemory(&blendStateDes, sizeof(D3D11_BLEND_DESC));

	blendStateDes.RenderTarget[0].BlendEnable = TRUE;
	blendStateDes.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDes.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDes.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDes.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDes.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDes.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDes.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	HRESULT hr = device->GetDevice()->CreateBlendState(&blendStateDes, _blendState.GetAddressOf());

	if (FAILED(hr))
		MessageBox(0, L"BlendState Failed.", 0, 0);
}

void BlendState::Release()
{
	_blendState.ReleaseAndGetAddressOf();
}
