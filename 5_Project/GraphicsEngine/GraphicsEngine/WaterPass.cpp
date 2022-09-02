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

	_light_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS"));		 // �̳���� �ϴ� �������� ���ؽ����̴��� ������
	_light_VS_1 = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS_1"));	 // ��Ų��

	_skyBox_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_VS"));
	_skyBox_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_PS"));


	_water_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Water_VS"));
	_water_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"WaterPassSky_PS"));

	//---------------------------------------------------------------------------------------------------------
	// water �� ���� renderTarget ����
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

	//// ���� ������Ʈ�� �� �ٲ㰡���� �������� �˴ϴ�.
	//// �׷��� nullptr �� �����ش�..
	g_deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);


	// ����Ÿ�ٺ�� �������ٽǺ並 ���ε� ���ش�.
	/*g_deviceContext->OMSetRenderTargets(WATER_COUNT, _gWaterViews.GetAddressOf(), _gWaterBuffers->GetDepthStencilView().Get());

	g_deviceContext->RSSetViewports(1, &(_gWaterBuffers->GetViewPort()));*/
}

void WaterPass::RenderDynamicCube(int i)
{

	g_deviceContext->RSSetViewports(1, &(_gWaterBuffers->GetCubeViewPort()));

	// ClearRenderTargetView �� ClearDepthStencilView�� �ȿ��� ���ÿ� �ϰ� �ִ�. 
	_gWaterBuffers->ClearCubeRenderTarget(i, g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	_gWaterBuffers->SetRenderTargetView(i);		// ���� Ÿ���� �ٲ��ش�. 

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

				// ��� �׸���
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
				GraphicsEngineAPI::GetInstance()->TurnSkyboxOn();	// ��ī�̹ڽ��� ����

				cbMesh cbMeshBuffer;
				Matrix worldTM = XMMatrixTranslationFromVector(RenderManager::s_waterCameraInfo[i].worldPos);
				cbMeshBuffer.gWorldViewProj = worldTM * view * Proj;
				cbMeshBuffer.objectID = mesh->materialID;

				_skyBox_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
				_skyBox_VS->Update();

				_skyBox_PS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
				_skyBox_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->cubeMapTexture, "gCubeMap");
				_skyBox_PS->Update();

				// ��� �׸���
				g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidNoneCullClass()->GetRasterizerState().Get());

				unsigned int stride = ResourceManager::GetInstance()->GetMesh(mesh->name)->stride;
				unsigned int offset = 0;

				// � ������� �׸���
				g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetPrimitiveTopology());

				// ���ؽ� ���� ����
				g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh(mesh->name)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

				// �ε��� ���� ����
				g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

				// �׸���!
				//if (!mesh->isBone)
				g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIdxBufferSize(), 0, 0);

				GraphicsEngineAPI::GetInstance()->TurnZBufferOn();

				continue;	// �����ɷ� �ֳĸ� �Ǳ׸��� �ȵǴϱ�
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

			// � ������� �׸���
			g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetPrimitiveTopology());

			// ���ؽ� ���� ����
			g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh(mesh->name)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

			// �ε��� ���� ����
			g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

			// �׸���!
			//if (!mesh->isBone)
			g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIdxBufferSize(), 0, 0);
		}
	}
}
