#include "pch.h"
#include "DebugPass.h"
#include "ResourceManager.h"
#include "ConstantBufferDefine.h"
#include "ShaderManager.h"
#include "RenderTargetView.h"
#include "DebugWindow.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderManager.h"
#include "LightManager.h"

DebugPass::DebugPass()
{}

DebugPass::~DebugPass()
{}

void DebugPass::Init()
{
	g_device = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice();
	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();

	_basic_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Basic_VS"));
	_basic_PS_1 = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Basic_PS_1"));	 // 텍스쳐가 있다면 이걸쓴다.
	_light_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS"));		 // 디폴트
	_light_VS_1 = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS_1"));	 // 스킨드 적용 
	
	_skyBox_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_VS"));
	_skyBox_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_PS"));

	_debug_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Debug_PS"));

	//---------------------------------------------------------------------------------------------------------
	//										Multi Render Target을 위한 초기화
	//---------------------------------------------------------------------------------------------------------
	for (int i = 0; i < 4; i++)
	{
		_renderTextureClass[i] = make_shared<RenderTargetView>(); 
		_renderTextureClass[i]->RenderTargetTextureInit(g_device, GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, GraphicsEngineAPI::GetInstance()->GetWindowInfo().height, DXGI_FORMAT_R32G32B32A32_FLOAT);

		_debugWindow[i] = make_shared<DebugWindow>();
		_debugWindow[i]->Init(g_device, 
			GraphicsEngineAPI::GetInstance()->GetWindowInfo().width,
			GraphicsEngineAPI::GetInstance()->GetWindowInfo().height,
			GraphicsEngineAPI::GetInstance()->GetWindowInfo().width / 5,
			GraphicsEngineAPI::GetInstance()->GetWindowInfo().height / 5);			// 이거 매개변수가 무쓸모인데? ㅋㅋ 일단 둠

		_renderTargetViewTexture[i] = _renderTextureClass[i]->GetRenderTargetView();
	}
}

void DebugPass::Release()
{
	for (int i = 0; i < 4; i++)
	{
		_renderTextureClass[i]->Release();

		_renderTargetViewTexture[i].ReleaseAndGetAddressOf();

		_debugWindow[i]->Release();
	}
}

void DebugPass::BeginRender()
{
	for (int i = 0; i < 4; i++)
	{
		_renderTextureClass[i]->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f + i * 0.3f, 1.0f, 1.0f));
	}

	// 블렌드 스테이트를 막 바꿔가지고 반투명이 됩니다.
	// 그래숴 nullptr 로 막아준다..
	g_deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	g_deviceContext->OMSetRenderTargets(4, _renderTargetViewTexture[0].GetAddressOf(), _renderTextureClass[0]->GetDepthStencilView().Get());

	g_deviceContext->RSSetViewports(1, &(_renderTextureClass[0]->GetViewPort()));
}

void DebugPass::Render(vector<shared_ptr<MeshInfo>> meshs)
{
	for (auto& mesh : meshs)
	{
		for (int matCnt = 0; matCnt < ResourceManager::GetInstance()->materials.size(); matCnt++)
		{
			//// 움 일단은 스카이박스는 디버그모드에서 렌더링 안하는걸루?
			//if (mesh->type == OBJECT_TYPE::SKY_BOX)
			//	continue;

			switch (mesh->type)
			{
				case OBJECT_TYPE::DEFAULT:
				{
					cbMesh cbMeshBuffer;
					cbMeshBuffer.gWorld = mesh->worldTM;
					cbMeshBuffer.gWorldViewProj = mesh->worldTM * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
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
					cbMesh cbMeshBuffer;
					Matrix camPos = XMMatrixTranslationFromVector(RenderManager::s_cameraInfo.worldPos);
					cbMeshBuffer.gWorldViewProj = camPos * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;

					_skyBox_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
					_skyBox_VS->Update();

					_skyBox_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->cubeMapTexture, "gCubeMap");
					_skyBox_PS->Update();

					// 어떻게 그릴지
					g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidNoneCullClass()->GetRasterizerState().Get());
				}
				break;
			
			}

			if (mesh->type != OBJECT_TYPE::SKY_BOX)
			{ 
				// 이 부분을 수정해 줘야한다. 
				//cbMaterialBuffer.gMaterialAmbient = mesh->materials[matCnt]->ambient;
				//cbMaterialBuffer.gMaterialDiffuse = mesh->materials[matCnt]->diffuse;
				//cbMaterialBuffer.gMaterialSpecular = mesh->materials[matCnt]->specular;
				//cbMaterialBuffer.isDiffuseTexture = mesh->materials[matCnt]->isDiffuse;
				//cbMaterialBuffer.isNormalTexture = mesh->materials[matCnt]->isNormal;
				//cbMaterialBuffer.isSpecularTexture = mesh->materials[matCnt]->isSpecular;


				if (ResourceManager::GetInstance()->materials[mesh->materialID]->isSpecular)
				{
					_debug_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->specularTexture, "gSpecularMap");
				}

				if (ResourceManager::GetInstance()->materials[mesh->materialID]->isNormal)
				{
					_debug_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->normalTexture, "gNormalMap");
				}

				if (ResourceManager::GetInstance()->materials[mesh->materialID]->isDiffuse)
				{
					_debug_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->diffuseTexture, "gDiffuseMap");
				}

				_debug_PS->ConstantBufferUpdate(&ResourceManager::cbMaterialBuffer, "cbMaterial");

				_debug_PS->Update();
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
			if (!mesh->isBone)
				g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIdxBufferSize(), 0, 0);
		}
	}
}

void DebugPass::EndRender()
{
	// 렌더링 타겟을 다시 백버퍼로 돌린다.
	GraphicsEngineAPI::GetInstance()->SetBackBufferRenderTarget();

	GraphicsEngineAPI::GetInstance()->TurnZBufferOff();

	// 디버그 윈도우의 정점과 인덱스 버퍼를 그래픽 파이프라인에 넣어 렌더링할 준비를 합니다. 
	for (int i = 0; i < 4; i++)
	{
		_debugWindow[i]->Render(g_deviceContext, Vector4(0.6f, 1.0f, 1.0f - i * 0.5f, 0.5f - i * 0.5f));	// 크기 얘로 조절

		g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidClass()->GetRasterizerState().Get());
		
		cbMesh cbMeshBuffer;
		cbMeshBuffer.gWorld = XMMatrixIdentity();
		cbMeshBuffer.gWorldViewProj = XMMatrixIdentity();
		_basic_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
		_basic_VS->Update();

		_basic_PS_1->SetResourceViewBuffer(_renderTextureClass[i]->GetSRV(), "gDiffuseMap");
		_basic_PS_1->Update();

		g_deviceContext->DrawIndexed(_debugWindow[i]->GetIndexCount(), 0, 0);
	}

	GraphicsEngineAPI::GetInstance()->TurnZBufferOn();
}
