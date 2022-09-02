#include "pch.h"
#include "ReflectionObjectPass.h"
#include "ResourceManager.h"
#include "RenderTargetView.h"
#include "DebugWindow.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "LightManager.h"


ReflectionObjectPass::ReflectionObjectPass()
{

}

ReflectionObjectPass::~ReflectionObjectPass()
{

}

void ReflectionObjectPass::Init()
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

	_gObjectBuffer = make_shared<RenderTargetView>();
	_gObjectBuffer->RenderTargetTextureInit(g_device, GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, GraphicsEngineAPI::GetInstance()->GetWindowInfo().height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	_gObjectBufferView = _gObjectBuffer->GetRenderTargetView();
}

void ReflectionObjectPass::Release()
{
	_gObjectBuffer->Release();
}

void ReflectionObjectPass::BeginRender()
{
	_gObjectBuffer->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	// ����Ÿ�ٺ�� �������ٽǺ並 ���ε� ���ش�.
	g_deviceContext->OMSetRenderTargets(1, _gObjectBufferView.GetAddressOf(), _gObjectBuffer->GetDepthStencilView().Get());

	g_deviceContext->RSSetViewports(1, &(_gObjectBuffer->GetViewPort()));
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ReflectionObjectPass::GetObjectSRV()
{
	return _gObjectBuffer->GetSRV();
}

void ReflectionObjectPass::Render(vector<shared_ptr<MeshInfo>> meshs)
{
	Vector3 meshpos = Vector3(0, 0, 0);
	Matrix reflectionview;
	for (auto& mesh : meshs)
	{
		switch (mesh->type)
		{
		case OBJECT_TYPE::DEFAULT:
		{
			cbMesh cbMeshBuffer;
			cbMeshBuffer.gWorld = mesh->worldTM;
			//cbMeshBuffer.gWorldViewProj = camPos * view * Proj; 
			cbMeshBuffer.gWorldViewProj = mesh->worldTM * RenderManager::s_waterCameraInfo[1].viewTM * RenderManager::s_waterCameraInfo[1].projTM;
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
		continue;
		}
		break;
		case OBJECT_TYPE::SKY_BOX:
		{
			continue;	// �����ɷ� �ֳĸ� �Ǳ׸��� �ȵǴϱ�
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
