#include "pch.h"
#include "RasterizerState.h"
#include "Device.h"

RasterizerState::RasterizerState()
	: _rasterizerState(nullptr)
{}

RasterizerState::~RasterizerState()
{}

void RasterizerState::Init(shared_ptr<Device> device, D3D11_CULL_MODE cullMode ,D3D11_FILL_MODE fillMode)
{
	// Render State Сп Rasterizer State
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.FillMode = fillMode;
	rasterDesc.CullMode = cullMode;

	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	device->GetDevice()->CreateRasterizerState(&rasterDesc, _rasterizerState.GetAddressOf());
}

void RasterizerState::Release()
{
	_rasterizerState.ReleaseAndGetAddressOf();
}
