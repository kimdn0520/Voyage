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
	// 이름이 넘 길어서 받아서 쓴다.
	g_device = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice();
	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();

	// 어떤 셰이더를 쓸지 불러온다~
	_basic_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Basic_VS"));
	_basic_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Basic_PS"));

	_particle_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Particle_PS"));

	_deferred_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Deferred_VS"));
	_deferred_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Deferred_PS"));
	_deferredMulti_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"DeferredMulti_PS"));

	_light_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS"));		 // 이놈들을 일단 전반적인 버텍스쉐이더로 쓸거임
	_light_VS_1 = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS_1"));	 // 스킨드

	_skyBox_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_VS"));
	_skyBox_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_PS"));

	_water_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Water_VS"));
	_water_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Water_PS"));

	//---------------------------------------------------------------------------------------------------------
	//										Deferred 초기화
	//---------------------------------------------------------------------------------------------------------

	for (int i = 0; i < DEFERRED_COUNT - 1; i++)
	{
		_gBuffers[i] = make_shared<RenderTargetView>();
		_gBuffers[i]->RenderTargetTextureInit(g_device, GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, GraphicsEngineAPI::GetInstance()->GetWindowInfo().height, DXGI_FORMAT_R32G32B32A32_FLOAT);
		//_gBuffers[i]->CubeMapRenderTargetTextureInit(g_device, DXGI_FORMAT_R8G8B8A8_UNORM);

		_gBufferViews[i] = _gBuffers[i]->GetRenderTargetView();

	}

	_gBuffers[DEFERRED_COUNT - 1] = make_shared<RenderTargetView>();
	// !!! 얘는 DXGI_FORMAT_R32_UINT로 해줘야해 SV_Target을 uint로 뽑을거기때문에
	_gBuffers[DEFERRED_COUNT - 1]->RenderTargetTextureInit(g_device, GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, GraphicsEngineAPI::GetInstance()->GetWindowInfo().height, DXGI_FORMAT_R32_UINT);
	_gBuffers[DEFERRED_COUNT - 1]->CubeMapRenderTargetTextureInit(g_device, DXGI_FORMAT_R8G8B8A8_UNORM);

	_gBufferViews[DEFERRED_COUNT - 1] = _gBuffers[DEFERRED_COUNT - 1]->GetRenderTargetView();

	_deferredWindow = make_shared<DebugWindow>();
	_deferredWindow->Init(g_device,
		GraphicsEngineAPI::GetInstance()->GetWindowInfo().width,
		GraphicsEngineAPI::GetInstance()->GetWindowInfo().height,
		GraphicsEngineAPI::GetInstance()->GetWindowInfo().width / 5,
		GraphicsEngineAPI::GetInstance()->GetWindowInfo().height / 5);			// 이거 매개변수가 무쓸모인데? ㅋㅋ 일단 둠
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
		_gBuffers[i]->CubeMapRenderTargetTextureInit(g_device, DXGI_FORMAT_R8G8B8A8_UNORM);	// 사실 width, height가 같아서 이걸 해주는 의미가 있나 싶다. 

		_gBufferViews[i].ReleaseAndGetAddressOf();

		_gBufferViews[i] = _gBuffers[i]->GetRenderTargetView();
	}

	// !!! 얘는 DXGI_FORMAT_R32_UINT로 해줘야해 SV_Target을 uint로 뽑을거기때문에
	_gBuffers[DEFERRED_COUNT - 1]->RenderTargetTextureInit(g_device, GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, GraphicsEngineAPI::GetInstance()->GetWindowInfo().height, DXGI_FORMAT_R32_UINT);
	_gBuffers[DEFERRED_COUNT - 1]->CubeMapRenderTargetTextureInit(g_device, DXGI_FORMAT_R8G8B8A8_UNORM);	// 사실 width, height가 같아서 이걸 해주는 의미가 있나 싶다. 

	_gBufferViews[DEFERRED_COUNT - 1].ReleaseAndGetAddressOf();
	_gBufferViews[DEFERRED_COUNT - 1] = _gBuffers[DEFERRED_COUNT - 1]->GetRenderTargetView();
}

void DeferredPass::BeginRender(/*OBJECT_TYPE type*/)
{
	for (int i = 0; i < DEFERRED_COUNT; i++)
	{
		_gBuffers[i]->ClearRenderTarget(g_deviceContext, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	// 블렌드 스테이트를 막 바꿔가지고 반투명이 됩니다.
	// 그래숴 nullptr 로 막아준다..
	g_deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	// Output Texture Unbind
	ID3D11ShaderResourceView* nullSRV[DEFERRED_COUNT] = { nullptr };

	g_deviceContext->PSSetShaderResources(0, DEFERRED_COUNT, nullSRV);

	// 렌더타겟뷰와 뎁스스텐실뷰를 바인딩 해준다.
	g_deviceContext->OMSetRenderTargets(DEFERRED_COUNT, _gBufferViews[0].GetAddressOf(), _gBuffers[0]->GetDepthStencilView().Get());

	g_deviceContext->RSSetViewports(1, &(_gBuffers[0]->GetViewPort()));
}

/// <summary>
/// MRT 하듯이 하면댐 텍스쳐에 다 그려놓고 그 렌더타겟의 SRV가져와서 조립한다!? 의 느낌쓰 
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

			// 어떻게 그릴지
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
			GraphicsEngineAPI::GetInstance()->TurnSkyboxOn();	// 스카이박스용 뎁스

			cbMesh cbMeshBuffer;
			Matrix camPos = XMMatrixTranslationFromVector(RenderManager::s_cameraInfo.worldPos);
			cbMeshBuffer.gWorldViewProj = camPos * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
			cbMeshBuffer.objectID = mesh->materialID;

			_skyBox_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_skyBox_VS->Update();

			// 흠 근데 이런경우는 어떻게 할까?
			// 요로콤 바꿔주면 된다. 기존에는 MeshInfo 안에 Material이 있었다. 
			_skyBox_PS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_skyBox_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->cubeMapTexture, "gCubeMap");
			_skyBox_PS->Update();

			// 어떻게 그릴지
			g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidNoneCullClass()->GetRasterizerState().Get());
			
			// Set vertex buffer stride and offset
			unsigned int stride = ResourceManager::GetInstance()->GetMesh(mesh->name)->stride;
			unsigned int offset = 0;

			// 어떤 방식으로 그릴지
			g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh(mesh->particleIdx)->GetPrimitiveTopology());

			// 버텍스 버퍼 설정
			g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh(mesh->particleIdx)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

			// 인덱스 버퍼 설정
			g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh(mesh->particleIdx)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

			// 그린다!
			//if (!mesh->isBone)
			g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh(mesh->particleIdx)->GetIdxBufferSize(), 0, 0);
			
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

			// 어떤 방식으로 그릴지
			g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetParticleMesh(mesh->particleIdx)->GetPrimitiveTopology());

			// 버텍스 버퍼 설정
			g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetParticleMesh(mesh->particleIdx)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

			// 인덱스 버퍼 설정
			g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetParticleMesh(mesh->particleIdx)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

			// 그린다!
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


void DeferredPass::EndRender()
{
	// 렌더링 타겟을 다시 백버퍼로 돌린다.
	GraphicsEngineAPI::GetInstance()->SetBackBufferRenderTarget();

	// z버퍼를 꺼준다.
	GraphicsEngineAPI::GetInstance()->TurnZBufferOff();

	// left ~ right : -1.0f ~ 1.0f 군요..!
	// top ~ bottom : 1.0f ~ -1.0f 군요..!
	_deferredWindow->Render(g_deviceContext, Vector4(-1.0f, 1.0f, 1.0f, -1.0f));	// 크기 얘로 조절

	// Solid로 설정
	g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidClass()->GetRasterizerState().Get());

	// 버텍스 쉐이더 업데이트
	_deferred_VS->Update();

	// 픽셀 쉐이더에 SRV 셋팅 
	_deferred_PS->SetResourceViewBuffer(_gBuffers[0]->GetSRV().Get(), "Albedo");
	_deferred_PS->SetResourceViewBuffer(_gBuffers[1]->GetSRV().Get(), "Depth");
	_deferred_PS->SetResourceViewBuffer(_gBuffers[2]->GetSRV().Get(), "Normal");
	_deferred_PS->SetResourceViewBuffer(_gBuffers[3]->GetSRV().Get(), "Position");
	_deferred_PS->SetResourceViewBuffer(_gBuffers[4]->GetSRV().Get(), "MaterialID");		// 빛을 받을지 안받을지 // 이름을 바꿔준다.

	/*_deferred_PS->SetResourceViewBuffer(_gReflectionSkySRV, "ReflectionSkyTexture");
	_deferred_PS->SetResourceViewBuffer(_gReflectionObjectSRV, "ReflectionObjectTexture");*/

	//_deferred_PS->SetResourceViewBuffer(ResourceManager::)

	_deferred_PS->ConstantBufferUpdate(&LightManager::cbLightBuffer, "cbLight");
	_deferred_PS->ConstantBufferUpdate(&ResourceManager::cbMaterialBuffer, "cbMaterial");

	_cbTextureBuf.textureInfo = Vector4(												// 이녀석으로 텍스쳐의 uint를 뽑으려고 텍스쳐 사이즈 건네줌
		static_cast<float>(GraphicsEngineAPI::GetInstance()->GetWindowInfo().width),
		static_cast<float>(GraphicsEngineAPI::GetInstance()->GetWindowInfo().height),
		1.f / GraphicsEngineAPI::GetInstance()->GetWindowInfo().width, 1.f / GraphicsEngineAPI::GetInstance()->GetWindowInfo().height);
	_deferred_PS->ConstantBufferUpdate(&_cbTextureBuf, "cbTexture");

	_deferred_PS->Update();

	// 그린다!
	g_deviceContext->DrawIndexed(_deferredWindow->GetIndexCount(), 0, 0);

	// z버퍼 다시 켜준다.
	GraphicsEngineAPI::GetInstance()->TurnZBufferOn();
}
