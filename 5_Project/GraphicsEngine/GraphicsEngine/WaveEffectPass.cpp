#include "pch.h"
#include "WaveEffectPass.h"
#include "ResourceManager.h"
#include "RenderTargetView.h"
#include "DebugWindow.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderTargetView.h"
#include "DebugWindow.h"
#include "BlendState.h"

WaveEffectPass::WaveEffectPass()
{

}

WaveEffectPass::~WaveEffectPass()
{

}

void WaveEffectPass::Init()
{
	// 이름이 넘 길어서 받아서 쓴다.
	g_device = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice();
	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();


	_waveEffect_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"WaveEffect_VS"));
	_waveEffect_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"WaveEffect_PS"));

}



void WaveEffectPass::BeginRender()
{
	g_deviceContext->OMSetBlendState(GraphicsEngineAPI::GetInstance()->GetAlphaBlendClass()->GetBlendState().Get(),
		nullptr, 0xffffffff);

}

void WaveEffectPass::Render(vector<shared_ptr<MeshInfo>> meshs)
{
	for (auto& mesh : meshs)
	{
		switch (mesh->type)
		{
		case OBJECT_TYPE::WATER:
		{
			continue;
		}
		break;
		case OBJECT_TYPE::WaveEffect:
		{
			cbMesh cbMeshBuffer;
			cbMeshBuffer.gWorldViewProj = mesh->worldTM * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
			//cbMeshBuffer.objectID = mesh->materialID;

			cbNoise cbNoiseBuffer;

			_deltaTime += 0.001f;
			if (_deltaTime > 1000.0f)
			{
				_deltaTime = 0.0f;
			}

			cbNoiseBuffer.frameTime = _deltaTime;
			cbNoiseBuffer.scales = Vector3(1.0f, 2.0f, 3.0f);
			cbNoiseBuffer.scrollSpeed = Vector3(1.3f, 2.1f, 2.3f);

			cbDistortion cbDistortionBuffer;

			// 세 가지 다른 노이즈 텍스처에 대해 세 가지 다른 x 및 y 왜곡 인수를 설정합니다.
			cbDistortionBuffer.distortion1 = Vector2(0.1f, 0.2f);
			cbDistortionBuffer.distortion2 = Vector2(0.1f, 0.3f);
			cbDistortionBuffer.distortion3 = Vector2(0.1f, 0.1f);

			cbDistortionBuffer.distortionScale = 0.8f;
			cbDistortionBuffer.distortionBias = 0.5f;


			_waveEffect_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_waveEffect_VS->ConstantBufferUpdate(&cbNoiseBuffer, "cbNoise");
			_waveEffect_VS->Update();

			std::wstring name1 = L"sea.png";
			std::wstring name2 = L"noise01.dds";
			std::wstring name3 = L"alpha01.dds";


			_waveEffect_PS->ConstantBufferUpdate(&cbDistortionBuffer, "cbDistortion");
			_waveEffect_PS->SetResourceViewBuffer(name1, "waveTexture");
			_waveEffect_PS->SetResourceViewBuffer(name2, "noiseTexture");
			_waveEffect_PS->SetResourceViewBuffer(name3, "alphaTexture");
			_waveEffect_PS->Update();

			// Set vertex buffer stride and offset
			unsigned int stride = ResourceManager::GetInstance()->GetMesh(mesh->name)->stride;
			unsigned int offset = 0;

			// 어떤 방식으로 그릴지
			g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetPrimitiveTopology());

			// 버텍스 버퍼 설정
			g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh(mesh->name)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

			// 인덱스 버퍼 설정
			g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

			// 그린다!
			//if (!mesh->isBone)
			g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIdxBufferSize(), 0, 0);
		}
		break;
		default:
			continue;
		}

		
	}
}

void WaveEffectPass::EndRender()
{
	g_deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

}

