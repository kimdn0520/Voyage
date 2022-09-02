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


	_light_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS"));		 // �̳���� �ϴ� �������� ���ؽ����̴��� ������
	_light_VS_1 = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS_1"));	 // ��Ų��

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

	// ����Ÿ�ٺ�� �������ٽǺ並 ���ε� ���ش�.
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
		// Bloom�� �޴� ������Ʈ�� �ƴϸ� Bloom�� ������ �ʴ´�. 
		if (mesh->isBloom == false)
			continue;
		
		// �ٽ� ����Ÿ���� �ʱ�ȭ�Ѵ�. 
		BeginRender();

		switch (mesh->type)
		{
		case OBJECT_TYPE::DEFAULT:
		{
			// �޴� ������Ʈ�� �ϴ� ���� Ÿ�Ͽ� �׷��ش�.
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

			// ��� �׸���
			g_deviceContext->RSSetState(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetRasterState().Get());


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

			// bloom �� ������Ʈ���� �ؽ��ĸ� ��´�. 
			_gObjectsSRV.push_back(_gBuffer->GetSRV());

		}
		break;
		case OBJECT_TYPE::UTIL:
			break;
			// ���� �ϴ��� Bloom�� ������ �ʴ´ٰ� �����ϰ� �ڵ带 «
			// ���� �ʿ��ϸ� �׳� �����ϴ°� �׸����
		case OBJECT_TYPE::SKY_BOX:
			break;
		case OBJECT_TYPE::WATER:
			break;
			break;
		}
	}
}