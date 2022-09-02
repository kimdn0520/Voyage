#include "pch.h"
#include "WaterPass.h"
#include "ResourceManager.h"
#include "RenderTargetView.h"
#include "DebugWindow.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "LightManager.h"

WaterPass::WaterPass()
{

}

WaterPass::~WaterPass()
{

}

void WaterPass::Init()
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

	//---------------------------------------------------------------------------------------------------------
	// water 를 위한 renderTarget 생성
	//---------------------------------------------------------------------------------------------------------

	_gWaterBuffers = make_shared<RenderTargetView>();
	_gWaterBuffers->CubeMapRenderTargetTextureInit(g_device, DXGI_FORMAT_R8G8B8A8_UNORM);

	_gWaterViews = _gWaterBuffers->GetRenderTargetView();

}

void WaterPass::Release()
{
	_gWaterBuffers->Release();

	_gWaterViews.ReleaseAndGetAddressOf();
}

void WaterPass::OnResize(int width, int height)
{
	//for (int i = 0; i < WATER_COUNT; i++)
	//{
	//_gWaterBuffers->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	//_gWaterViews.ReleaseAndGetAddressOf();

	//_gWaterViews = _gWaterBuffers->GetRenderTargetView();
	//}
}

void WaterPass::BeginRender()
{
	//_gWaterBuffers->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	//// 블렌드 스테이트를 막 바꿔가지고 반투명이 됩니다.
	//// 그래숴 nullptr 로 막아준다..
	g_deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);


	// 렌더타겟뷰와 뎁스스텐실뷰를 바인딩 해준다.
	/*g_deviceContext->OMSetRenderTargets(WATER_COUNT, _gWaterViews.GetAddressOf(), _gWaterBuffers->GetDepthStencilView().Get());

	g_deviceContext->RSSetViewports(1, &(_gWaterBuffers->GetViewPort()));*/
}

void WaterPass::RenderDynamicCube(int i)
{

	g_deviceContext->RSSetViewports(1, &(_gWaterBuffers->GetCubeViewPort()));

	// ClearRenderTargetView 와 ClearDepthStencilView는 안에서 동시에 하고 있다. 
	_gWaterBuffers->ClearCubeRenderTarget(i, g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	_gWaterBuffers->SetRenderTargetView(i);		// 렌더 타켓을 바꿔준다. 

	g_deviceContext->OMSetRenderTargets(1, _gWaterBuffers->GetRenderTargetView().GetAddressOf(), _gWaterBuffers->GetCubeDepthStencilView().Get());

}


ComPtr<ID3D11ShaderResourceView> WaterPass::GetWaterSRV()
{
	g_deviceContext->GenerateMips(_gWaterBuffers->GetCubeSRV().Get());
	return _gWaterBuffers->GetCubeSRV();
}

void WaterPass::Render(vector<shared_ptr<MeshInfo>> meshs)
{
	for (int i = 0; i < 6; ++i)
	{
		RenderDynamicCube(i);
		Matrix view = RenderManager::s_waterCameraInfo[i].viewTM;
		Matrix Proj = RenderManager::s_waterCameraInfo[i].projTM;
		
		for (auto& mesh : meshs)
		{ 
			switch (mesh->type)
			{
			case OBJECT_TYPE::DEFAULT:
			{
				cbMesh cbMeshBuffer;
				cbMeshBuffer.gWorld = mesh->worldTM;
				cbMeshBuffer.gWorldViewProj = mesh->worldTM * view * Proj;
				cbMeshBuffer.objectID = mesh->materialID;
				XMVECTOR det = XMMatrixDeterminant(mesh->worldTM);
				cbMeshBuffer.gWorldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, mesh->worldTM));

				if (mesh->isSkinned)
				{
					cbSkinned cbSkinnedBuffer;
					memcpy(&cbSkinnedBuffer.gBoneTransforms, mesh->finalBoneListMatrix, sizeof(Matrix) * 96);
					_light_VS_1->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
					_light_VS_1->ConstantBufferUpdate(&cbSkinnedBuffer, "cbSkinned");
					_light_VS_1->Update();
				}
				else
				{
					_light_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
					_light_VS->Update();
				}


				_waterPassDefault->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
				_waterPassDefault->ConstantBufferUpdate(&ResourceManager::GetInstance()->cbMaterialBuffer, "cbMaterial");
				
				if (ResourceManager::GetInstance()->materials[mesh->materialID]->isDiffuse)
				{
					_waterPassDefault->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->diffuseTexture, "gDiffuseMap");
				}

				if (ResourceManager::GetInstance()->materials[mesh->materialID]->isNormal)
				{
					_waterPassDefault->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->normalTexture, "gNormalMap");
				}

				if (ResourceManager::GetInstance()->materials[mesh->materialID]->isSpecular)
				{
					_waterPassDefault->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->specularTexture, "gSpecularMap");
				}

				_waterPassDefault->Update();

				// 어떻게 그릴지
				g_deviceContext->RSSetState(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetRasterState().Get());
			}
			break;
			case OBJECT_TYPE::UTIL:
			{
				// TODO : ..
			}
			break;
			case OBJECT_TYPE::SKY_BOX:
			{
				GraphicsEngineAPI::GetInstance()->TurnSkyboxOn();	// 스카이박스용 뎁스

				cbMesh cbMeshBuffer;
				Matrix worldTM = XMMatrixTranslationFromVector(RenderManager::s_waterCameraInfo[i].worldPos);
				cbMeshBuffer.gWorldViewProj = worldTM * view * Proj;
				cbMeshBuffer.objectID = mesh->materialID;

				_skyBox_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
				_skyBox_VS->Update();

				_skyBox_PS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
				_skyBox_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->cubeMapTexture, "gCubeMap");
				_skyBox_PS->Update();

				// 어떻게 그릴지
				g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidNoneCullClass()->GetRasterizerState().Get());

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
}
