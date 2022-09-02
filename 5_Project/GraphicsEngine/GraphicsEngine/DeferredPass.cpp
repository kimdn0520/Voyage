#include "pch.h"
#include "DeferredPass.h"
#include "ResourceManager.h"
#include "RenderTargetView.h"
#include "DebugWindow.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "LightManager.h"

DeferredPass::DeferredPass()
{}

DeferredPass::~DeferredPass()
{}

void DeferredPass::Init()
{
	// �̸��� �� �� �޾Ƽ� ����.
	g_device = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice();
	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();

	// � ���̴��� ���� �ҷ��´�~
	_basic_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Basic_VS"));
	_basic_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Basic_PS"));

	_particle_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Particle_PS"));

	_deferred_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Deferred_VS"));
	_deferred_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Deferred_PS"));
	_deferredMulti_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"DeferredMulti_PS"));

	_light_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS"));		 // �̳���� �ϴ� �������� ���ؽ����̴��� ������
	_light_VS_1 = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS_1"));	 // ��Ų��

	_skyBox_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_VS"));
	_skyBox_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_PS"));

	_water_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Water_VS"));
	_water_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Water_PS"));

	//---------------------------------------------------------------------------------------------------------
	//										Deferred �ʱ�ȭ
	//---------------------------------------------------------------------------------------------------------

	for (int i = 0; i < DEFERRED_COUNT - 1; i++)
	{
		_gBuffers[i] = make_shared<RenderTargetView>();
		_gBuffers[i]->RenderTargetTextureInit(g_device, GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, GraphicsEngineAPI::GetInstance()->GetWindowInfo().height, DXGI_FORMAT_R32G32B32A32_FLOAT);
		//_gBuffers[i]->CubeMapRenderTargetTextureInit(g_device, DXGI_FORMAT_R8G8B8A8_UNORM);

		_gBufferViews[i] = _gBuffers[i]->GetRenderTargetView();

	}

	_gBuffers[DEFERRED_COUNT - 1] = make_shared<RenderTargetView>();
	// !!! ��� DXGI_FORMAT_R32_UINT�� ������� SV_Target�� uint�� �����ű⶧����
	_gBuffers[DEFERRED_COUNT - 1]->RenderTargetTextureInit(g_device, GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, GraphicsEngineAPI::GetInstance()->GetWindowInfo().height, DXGI_FORMAT_R32_UINT);
	_gBuffers[DEFERRED_COUNT - 1]->CubeMapRenderTargetTextureInit(g_device, DXGI_FORMAT_R8G8B8A8_UNORM);

	_gBufferViews[DEFERRED_COUNT - 1] = _gBuffers[DEFERRED_COUNT - 1]->GetRenderTargetView();

	_deferredWindow = make_shared<DebugWindow>();
	_deferredWindow->Init(g_device,
		GraphicsEngineAPI::GetInstance()->GetWindowInfo().width,
		GraphicsEngineAPI::GetInstance()->GetWindowInfo().height,
		GraphicsEngineAPI::GetInstance()->GetWindowInfo().width / 5,
		GraphicsEngineAPI::GetInstance()->GetWindowInfo().height / 5);			// �̰� �Ű������� �������ε�? ���� �ϴ� ��
}

void DeferredPass::Release()
{
	for (int i = 0; i < DEFERRED_COUNT; i++)
	{
		_gBuffers[i]->Release();

		//_gBufferViews[i].ReleaseAndGetAddressOf();
	}

	_deferredWindow->Release();
}

void DeferredPass::OnResize(int width, int height)
{
	for (int i = 0; i < DEFERRED_COUNT - 1; i++)
	{
		_gBuffers[i]->RenderTargetTextureInit(g_device, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
		_gBuffers[i]->CubeMapRenderTargetTextureInit(g_device, DXGI_FORMAT_R8G8B8A8_UNORM);	// ��� width, height�� ���Ƽ� �̰� ���ִ� �ǹ̰� �ֳ� �ʹ�. 

		_gBufferViews[i].ReleaseAndGetAddressOf();

		_gBufferViews[i] = _gBuffers[i]->GetRenderTargetView();
	}

	// !!! ��� DXGI_FORMAT_R32_UINT�� ������� SV_Target�� uint�� �����ű⶧����
	_gBuffers[DEFERRED_COUNT - 1]->RenderTargetTextureInit(g_device, GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, GraphicsEngineAPI::GetInstance()->GetWindowInfo().height, DXGI_FORMAT_R32_UINT);
	_gBuffers[DEFERRED_COUNT - 1]->CubeMapRenderTargetTextureInit(g_device, DXGI_FORMAT_R8G8B8A8_UNORM);	// ��� width, height�� ���Ƽ� �̰� ���ִ� �ǹ̰� �ֳ� �ʹ�. 

	_gBufferViews[DEFERRED_COUNT - 1].ReleaseAndGetAddressOf();
	_gBufferViews[DEFERRED_COUNT - 1] = _gBuffers[DEFERRED_COUNT - 1]->GetRenderTargetView();
}

void DeferredPass::BeginRender(/*OBJECT_TYPE type*/)
{
	for (int i = 0; i < DEFERRED_COUNT; i++)
	{
		_gBuffers[i]->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	// ���� ������Ʈ�� �� �ٲ㰡���� �������� �˴ϴ�.
	// �׷��� nullptr �� �����ش�..
	g_deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	// Output Texture Unbind
	ID3D11ShaderResourceView* nullSRV[DEFERRED_COUNT] = { nullptr };

	g_deviceContext->PSSetShaderResources(0, DEFERRED_COUNT, nullSRV);

	// ����Ÿ�ٺ�� �������ٽǺ並 ���ε� ���ش�.
	g_deviceContext->OMSetRenderTargets(DEFERRED_COUNT, _gBufferViews[0].GetAddressOf(), _gBuffers[0]->GetDepthStencilView().Get());

	g_deviceContext->RSSetViewports(1, &(_gBuffers[0]->GetViewPort()));
}

/// <summary>
/// MRT �ϵ��� �ϸ�� �ؽ��Ŀ� �� �׷����� �� ����Ÿ���� SRV�����ͼ� �����Ѵ�!? �� ������ 
/// </summary>
void DeferredPass::Render(vector<shared_ptr<MeshInfo>> meshs)
{
	for (auto& mesh : meshs)
	{
		switch (mesh->type)
		{
		case OBJECT_TYPE::DEFAULT:
		{
			cbMesh cbMeshBuffer;
			cbMeshBuffer.gWorld = mesh->worldTM;
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

			// ��� �׸���
			g_deviceContext->RSSetState(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetRasterState().Get());

			if (ResourceManager::GetInstance()->materials[mesh->materialID]->isDiffuse)
			{
				_deferredMulti_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->diffuseTexture, "gDiffuseMap");
			}

			if (ResourceManager::GetInstance()->materials[mesh->materialID]->isNormal)
			{
				_deferredMulti_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->normalTexture, "gNormalMap");
			}

			if (ResourceManager::GetInstance()->materials[mesh->materialID]->isSpecular)
			{
				_deferredMulti_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->specularTexture, "gSpecularMap");
			}

			/*_deferredMulti_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->cubeMapTexture, "gCubeMap");*/

			_deferredMulti_PS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_deferredMulti_PS->ConstantBufferUpdate(&ResourceManager::GetInstance()->cbMaterialBuffer, "cbMaterial");
			//_deferredMulti_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->, "ReflectionNomalMap");


			_deferredMulti_PS->Update();
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
			Matrix camPos = XMMatrixTranslationFromVector(RenderManager::s_cameraInfo.worldPos);
			cbMeshBuffer.gWorldViewProj = camPos * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
			cbMeshBuffer.objectID = mesh->materialID;

			_skyBox_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_skyBox_VS->Update();

			// �� �ٵ� �̷����� ��� �ұ�?
			// ����� �ٲ��ָ� �ȴ�. �������� MeshInfo �ȿ� Material�� �־���. 
			_skyBox_PS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_skyBox_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->cubeMapTexture, "gCubeMap");
			_skyBox_PS->Update();

			// ��� �׸���
			g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidNoneCullClass()->GetRasterizerState().Get());
			
			// Set vertex buffer stride and offset
			unsigned int stride = ResourceManager::GetInstance()->GetMesh(mesh->name)->stride;
			unsigned int offset = 0;

			// � ������� �׸���
			g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh(mesh->particleIdx)->GetPrimitiveTopology());

			// ���ؽ� ���� ����
			g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh(mesh->particleIdx)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

			// �ε��� ���� ����
			g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh(mesh->particleIdx)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

			// �׸���!
			//if (!mesh->isBone)
			g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh(mesh->particleIdx)->GetIdxBufferSize(), 0, 0);
			
			GraphicsEngineAPI::GetInstance()->TurnZBufferOn();

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
			cbMesh cbMeshBuffer;
			cbMeshBuffer.gWorldViewProj = mesh->worldTM * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
			cbMeshBuffer.objectID = mesh->materialID;

			_basic_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_basic_VS->Update();

			_particle_PS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			//_particle_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->diffuseTexture, "gDiffuseMap");
			//_particle_PS->SetResourceViewBuffer(L"skyMap.png", "gDiffuseMap");

			_particle_PS->Update();

			// Set vertex buffer stride and offset
			unsigned int stride = ResourceManager::GetInstance()->GetParticleMesh(mesh->particleIdx)->stride;
			unsigned int offset = 0;

			// � ������� �׸���
			g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetParticleMesh(mesh->particleIdx)->GetPrimitiveTopology());

			// ���ؽ� ���� ����
			g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetParticleMesh(mesh->particleIdx)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

			// �ε��� ���� ����
			g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetParticleMesh(mesh->particleIdx)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

			// �׸���!
			//if (!mesh->isBone)
			g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetParticleMesh(mesh->particleIdx)->GetIdxBufferSize(), 0, 0);

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


void DeferredPass::EndRender()
{
	// ������ Ÿ���� �ٽ� ����۷� ������.
	GraphicsEngineAPI::GetInstance()->SetBackBufferRenderTarget();

	// z���۸� ���ش�.
	GraphicsEngineAPI::GetInstance()->TurnZBufferOff();

	// left ~ right : -1.0f ~ 1.0f ����..!
	// top ~ bottom : 1.0f ~ -1.0f ����..!
	_deferredWindow->Render(g_deviceContext, Vector4(-1.0f, 1.0f, 1.0f, -1.0f));	// ũ�� ��� ����

	// Solid�� ����
	g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidClass()->GetRasterizerState().Get());

	// ���ؽ� ���̴� ������Ʈ
	_deferred_VS->Update();

	// �ȼ� ���̴��� SRV ���� 
	_deferred_PS->SetResourceViewBuffer(_gBuffers[0]->GetSRV().Get(), "Albedo");
	_deferred_PS->SetResourceViewBuffer(_gBuffers[1]->GetSRV().Get(), "Depth");
	_deferred_PS->SetResourceViewBuffer(_gBuffers[2]->GetSRV().Get(), "Normal");
	_deferred_PS->SetResourceViewBuffer(_gBuffers[3]->GetSRV().Get(), "Position");
	_deferred_PS->SetResourceViewBuffer(_gBuffers[4]->GetSRV().Get(), "MaterialID");		// ���� ������ �ȹ����� // �̸��� �ٲ��ش�.

	/*_deferred_PS->SetResourceViewBuffer(_gReflectionSkySRV, "ReflectionSkyTexture");
	_deferred_PS->SetResourceViewBuffer(_gReflectionObjectSRV, "ReflectionObjectTexture");*/

	//_deferred_PS->SetResourceViewBuffer(ResourceManager::)

	_deferred_PS->ConstantBufferUpdate(&LightManager::cbLightBuffer, "cbLight");
	_deferred_PS->ConstantBufferUpdate(&ResourceManager::cbMaterialBuffer, "cbMaterial");

	_cbTextureBuf.textureInfo = Vector4(												// �̳༮���� �ؽ����� uint�� �������� �ؽ��� ������ �ǳ���
		static_cast<float>(GraphicsEngineAPI::GetInstance()->GetWindowInfo().width),
		static_cast<float>(GraphicsEngineAPI::GetInstance()->GetWindowInfo().height),
		1.f / GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, 1.f / GraphicsEngineAPI::GetInstance()->GetWindowInfo().height);
	_deferred_PS->ConstantBufferUpdate(&_cbTextureBuf, "cbTexture");

	_deferred_PS->Update();

	// �׸���!
	g_deviceContext->DrawIndexed(_deferredWindow->GetIndexCount(), 0, 0);

	// z���� �ٽ� ���ش�.
	GraphicsEngineAPI::GetInstance()->TurnZBufferOn();
}
