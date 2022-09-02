#include "pch.h"
#include "PreBloomPass.h"
#include "ResourceManager.h"
#include "RenderTargetView.h"
#include "DebugWindow.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "LightManager.h"

PreBloomPass::PreBloomPass()
{

}

PreBloomPass::~PreBloomPass()
{

}

void PreBloomPass::Init()
{
	g_device = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice();
	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();

	_deferred_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Deferred_VS"));
	_waterPassDefault = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"WaterPassDefault_PS"));


	_light_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS"));		 // 이놈들을 일단 전반적인 버텍스쉐이더로 쓸거임
	_light_VS_1 = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS_1"));	 // 스킨드

	_skyBox_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_VS"));
	_skyBox_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_PS"));

	_gBuffer = make_shared<RenderTargetView>();
	_gBuffer->RenderTargetTextureInit(g_device, GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, GraphicsEngineAPI::GetInstance()->GetWindowInfo().height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	_gBufferView = _gBuffer->GetRenderTargetView();
}

void PreBloomPass::Release()
{
	_gBuffer->Release();
}

void PreBloomPass::BeginRender()
{
	_gBuffer->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	// 렌더타겟뷰와 뎁스스텐실뷰를 바인딩 해준다.
	g_deviceContext->OMSetRenderTargets(1, _gBufferView.GetAddressOf(), _gBuffer->GetDepthStencilView().Get());

	g_deviceContext->RSSetViewports(1, &(_gBuffer->GetViewPort()));
}

std::vector<ComPtr<ID3D11ShaderResourceView>> PreBloomPass::GetBloomObjectsSRV()
{
	return _gObjectsSRV;
}

void PreBloomPass::Render(vector<shared_ptr<MeshInfo>> meshs)
{
	for (auto& mesh : meshs)
	{
		// Bloom을 받는 오브젝트가 아니면 Bloom을 먹이지 않는다. 
		if (mesh->isBloom == false)
			continue;
		
		// 다시 렌더타켓을 초기화한다. 
		BeginRender();

		switch (mesh->type)
		{
		case OBJECT_TYPE::DEFAULT:
		{
			// 받는 오브젝트면 일단 렌더 타켓에 그려준다.
			cbMesh cbMeshBuffer;
			cbMeshBuffer.gWorld = mesh->worldTM;
			//cbMeshBuffer.gWorldViewProj = camPos * view * Proj; 
			cbMeshBuffer.gWorldViewProj = mesh->worldTM * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
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

			// bloom 할 오브젝트들의 텍스쳐를 담는다. 
			_gObjectsSRV.push_back(_gBuffer->GetSRV());

		}
		break;
		case OBJECT_TYPE::UTIL:
			break;
			// 물과 하늘은 Bloom을 먹이지 않는다고 생각하고 코드를 짬
			// 만약 필요하면 그냥 렌더하는거 그리면됨
		case OBJECT_TYPE::SKY_BOX:
			break;
		case OBJECT_TYPE::WATER:
			break;
			break;
		}
	}
}