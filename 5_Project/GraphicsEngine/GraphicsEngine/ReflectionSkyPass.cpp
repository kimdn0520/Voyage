#include "pch.h"
#include "ReflectionSkyPass.h"
#include "ResourceManager.h"
#include "RenderTargetView.h"
#include "DebugWindow.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "LightManager.h"

Matrix ReflectionSkyPass::reflectionView;

ReflectionSkyPass::ReflectionSkyPass()
{

}

ReflectionSkyPass::~ReflectionSkyPass()
{

}

void ReflectionSkyPass::Init()
{
	g_device = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice();
	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();

	_deferred_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Deferred_VS"));
	_waterPassDefault = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"WaterPassDefault_PS"));


	_light_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS"));		 // 이놈들을 일단 전반적인 버텍스쉐이더로 쓸거임
	_light_VS_1 = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS_1"));	 // 스킨드

	_skyBox_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_VS"));
	_skyBox_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_PS"));

	_water_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Water_VS"));
	_water_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"WaterPassSky_PS"));


	_gSkyBuffer = make_shared<RenderTargetView>();
	_gSkyBuffer->RenderTargetTextureInit(g_device, GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, GraphicsEngineAPI::GetInstance()->GetWindowInfo().height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	_gSkyBufferView = _gSkyBuffer->GetRenderTargetView();

}

void ReflectionSkyPass::Release()
{
	_gSkyBuffer->Release();
	
}

void ReflectionSkyPass::BeginRender()
{
	_gSkyBuffer->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	// 렌더타겟뷰와 뎁스스텐실뷰를 바인딩 해준다.
	g_deviceContext->OMSetRenderTargets(1, _gSkyBufferView.GetAddressOf(), _gSkyBuffer->GetDepthStencilView().Get());

	g_deviceContext->RSSetViewports(1, &(_gSkyBuffer->GetViewPort()));
}


Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ReflectionSkyPass::GetSkySRV()
{
	return _gSkyBuffer->GetSRV();
}

void ReflectionSkyPass::Render(vector<shared_ptr<MeshInfo>> meshs)
{
	Vector3 meshpos = Vector3(0, 0, 0);
	Matrix reflectionview;
	for (auto& mesh : meshs)
	{
		switch (mesh->type)
		{
		case OBJECT_TYPE::DEFAULT:
		{
			continue;
		}
		break;
		case OBJECT_TYPE::UTIL:
		{
		}
		break;
		case OBJECT_TYPE::SKY_BOX:
		{
			GraphicsEngineAPI::GetInstance()->TurnSkyboxOn();	// 스카이박스용 뎁스
			cbMesh cbMeshBuffer;
			Matrix camPos = XMMatrixTranslationFromVector(RenderManager::s_waterCameraInfo[2].worldPos);
			Matrix view = RenderManager::s_waterCameraInfo[2].viewTM;
			Matrix Proj = RenderManager::s_waterCameraInfo[2].projTM;
			cbMeshBuffer.gWorldViewProj = camPos * view * Proj;
			cbMeshBuffer.objectID = mesh->materialID;

			_skyBox_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_skyBox_VS->Update();

			// 흠 근데 이런경우는 어떻게 할까?
			// 요로콤 바꿔주면 된다. 기존에는 MeshInfo 안에 Material이 있었다. 
			_water_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->cubeMapTexture, "gCubeMap");
			_water_PS->Update();

			// 어떻게 그릴지
			g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidNoneCullClass()->GetRasterizerState().Get());

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

			GraphicsEngineAPI::GetInstance()->TurnZBufferOn();

			continue;	// 다음걸로 왜냐면 또그리면 안되니까
		}
		break;
		case OBJECT_TYPE::WATER:
		{
			continue;
		}
		break;
		case OBJECT_TYPE::PARTICLE:
		{
			continue;
		}
		break;
		case OBJECT_TYPE::WaveEffect:
		{
			continue;
		}
		break;
		}

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
}

