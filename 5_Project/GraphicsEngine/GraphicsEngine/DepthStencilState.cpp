#include "pch.h"
#include "DepthStencilState.h"
#include "Device.h"

DepthStencilState::DepthStencilState()
	: _depthStencilState(nullptr)
{}

DepthStencilState::~DepthStencilState()
{}

void DepthStencilState::Init(shared_ptr<Device> device, bool depthEnable, D3D11_COMPARISON_FUNC depthFuc)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDesc.DepthEnable = depthEnable;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = depthFuc;									// ��ī�̹ڽ� �Ҷ� ���� LESS_EQUAL���ؾ��� 
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	// Stencil operations if pixel is front-facing. 
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing. 
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	device->GetDevice()->CreateDepthStencilState(&depthStencilDesc, _depthStencilState.GetAddressOf());
}

void DepthStencilState::Release()
{
	_depthStencilState.ReleaseAndGetAddressOf();
}
