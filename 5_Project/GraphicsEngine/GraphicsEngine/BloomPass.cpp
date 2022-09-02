#include "pch.h"
#include "BloomPass.h"
#include "ResourceManager.h"
#include "RenderTargetView.h"
#include "DebugWindow.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "LightManager.h"

BloomPass::BloomPass()
{

}

BloomPass::~BloomPass()
{

}

void BloomPass::Init()
{
	g_device = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice();
	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();

	int width = GraphicsEngineAPI::GetInstance()->GetWindowInfo().width / 4;
	int height = GraphicsEngineAPI::GetInstance()->GetWindowInfo().height / 4;

	// DownSampling�� ���� Ÿ���� �����Ѵ�.
	_DownSampleBuffer[0] = make_shared<RenderTargetView>();
	_DownSampleBuffer[0]->RenderTargetTextureInit(g_device, width / 4, height / 4, DXGI_FORMAT_R32G32B32A32_FLOAT);

	for (int i = 0; i < 2; ++i)
	{
		width = width / 6;
		height = height / 6;

		_DownSampleBuffer[i + 1] = make_shared<RenderTargetView>();
		_DownSampleBuffer[i + 1]->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	}

	_DownSampling_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"DownSampling_PS"));
}

void BloomPass::Release()
{

}

void BloomPass::BeginRender()
{

}

// ������ �����ɷ� ����
void BloomPass::Render(vector<shared_ptr<MeshInfo>> meshs)
{
	for (auto bloomtexture : _bloomObjects)
	{
		// Bloom�� ������ �����ϴ� ���ÿ� DownSampling�� �������ش�. 
		_DownSampling_PS->SetResourceViewBuffer(bloomtexture, "CurveTexture");
		_DownSampling_PS->Update();

	}
}

void BloomPass::SetMainSRV()
{
	_mainRenderTargetView = GraphicsEngineAPI::GetInstance()->GetMainRenderTarget();
	_mainSRV = _mainRenderTargetView->GetSRV();
}

/// Bloom�� ������Ʈ���� ��� �����ϰ� �ִٰ� �������� ��� ������Ʈ�� �����ִ� �۾��� �Ѵ�.
void BloomPass::EndRender()
{
	SetMainSRV();

}
