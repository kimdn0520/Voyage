#include "pch.h"
#include "UIPass.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "ConstantBufferDefine.h"
#include "BlendState.h"
#include "RenderTargetView.h"
#include "DebugWindow.h"

float UIPass::alpha = 0.f;
bool UIPass::isFade = false;

UIPass::UIPass()
{}

UIPass::~UIPass()
{}

void UIPass::Init()
{
	g_graphics = GraphicsEngineAPI::GetInstance();
	g_device = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice();
	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();

	_UI_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"UI_VS"));
	_UI_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"UI_PS"));

	_basic_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Basic_VS"));
	_basic_PS_1 = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Basic_PS_1"));	 // 텍스쳐가 있다면 이걸쓴다.

	_renderTextureClass = make_shared<RenderTargetView>();
	_renderTextureClass->RenderTargetTextureInit(g_device, g_graphics->GetWindowInfo().width, g_graphics->GetWindowInfo().height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	_debugWindow = make_shared<DebugWindow>();
	_debugWindow->Init(g_device,
		GraphicsEngineAPI::GetInstance()->GetWindowInfo().width,
		GraphicsEngineAPI::GetInstance()->GetWindowInfo().height,
		GraphicsEngineAPI::GetInstance()->GetWindowInfo().width / 5,
		GraphicsEngineAPI::GetInstance()->GetWindowInfo().height / 5);			

	_renderTargetViewTexture = _renderTextureClass->GetRenderTargetView();
}

void UIPass::Release()
{
	_renderTextureClass->Release();

	_renderTargetViewTexture.ReleaseAndGetAddressOf();

	_debugWindow->Release();
}

void UIPass::BeginRender()
{
	g_deviceContext->OMSetBlendState(GraphicsEngineAPI::GetInstance()->GetAlphaBlendClass()->GetBlendState().Get(),
									 nullptr, 0xffffffff);

	// 2d image 니까 뎁스 꺼줘야해!!
	GraphicsEngineAPI::GetInstance()->TurnZBufferOff();
}

void UIPass::Render(vector<shared_ptr<UIInfo>> uiInfos)
{
	// ui sortlayer 따라 정렬해준다.
	sort(uiInfos.begin(), uiInfos.end(), [](shared_ptr<UIInfo>& a, shared_ptr<UIInfo>& b) { return a->sortLayer < b->sortLayer; });

	for (auto& uiInfo : uiInfos)
	{
		switch (uiInfo->uiType)
		{
			case UIType::Button:
			{
				cbMesh cbMeshBuffer;
				Matrix worldTM = XMMatrixIdentity();
				Matrix viewTM = XMMatrixIdentity();
				viewTM(0, 0) *= (uiInfo->right_Offset - uiInfo->left_Offset) / 2;
				viewTM(1, 1) *= (uiInfo->bottom_Offset - uiInfo->top_Offset) / 2;
				viewTM(2, 2) *= 1.f;
				
				viewTM(3, 0) = -(uiInfo->width / 2) + uiInfo->left_Offset + viewTM(0, 0);
				viewTM(3, 1) = (uiInfo->height / 2) - uiInfo->top_Offset - viewTM(1, 1);

				cbMeshBuffer.gWorldViewProj = worldTM * viewTM * RenderManager::s_cameraInfo.orthoProjTM;

				_UI_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
				_UI_VS->Update();

				if (uiInfo->status == ButtonStatus::NORMAL) 
				{
					_UI_PS->SetResourceViewBuffer(uiInfo->normalImage, "gDiffuseMap");
				}
				else if (uiInfo->status == ButtonStatus::HIGHLIGHT)
				{
					_UI_PS->SetResourceViewBuffer(uiInfo->highLightImage, "gDiffuseMap");
				}
				else if (uiInfo->status == ButtonStatus::PRESSED)
				{
					_UI_PS->SetResourceViewBuffer(uiInfo->pressedImage, "gDiffuseMap");
				}

				cbUI cbui;
				cbui.isAlpha = uiInfo->isAlpha;
				cbui.alpha = uiInfo->alpha;

				_UI_PS->ConstantBufferUpdate(&cbui, "cbUI");
				_UI_PS->Update();

				// 어떻게 그릴지.. ui는 solid?
				g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidClass()->GetRasterizerState().Get());

				unsigned int stride = ResourceManager::GetInstance()->GetMesh((int)MeshName::ScreenMesh)->stride;
				unsigned int offset = 0;

				// 어떤 방식으로 그릴지
				g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh((int)MeshName::ScreenMesh)->GetPrimitiveTopology());

				// 버텍스 버퍼 설정
				g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh((int)MeshName::ScreenMesh)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

				// 인덱스 버퍼 설정
				g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh((int)MeshName::ScreenMesh)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

				// 그린다!
				g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh((int)MeshName::ScreenMesh)->GetIdxBufferSize(), 0, 0);
			}
			break;
			case UIType::Panel:
			{
				cbMesh cbMeshBuffer;
				Matrix worldTM = XMMatrixIdentity();
				Matrix viewTM = XMMatrixIdentity();
				viewTM(0, 0) *= (uiInfo->right_Offset - uiInfo->left_Offset) / 2;
				viewTM(1, 1) *= (uiInfo->bottom_Offset - uiInfo->top_Offset) / 2;
				viewTM(2, 2) *= 1.f;

				viewTM(3, 0) = -(uiInfo->width / 2) + uiInfo->left_Offset + viewTM(0, 0);
				viewTM(3, 1) = (uiInfo->height / 2) - uiInfo->top_Offset - viewTM(1, 1);

				cbMeshBuffer.gWorldViewProj = worldTM * viewTM * RenderManager::s_cameraInfo.orthoProjTM;

				_UI_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
				_UI_VS->Update();
				
				cbUI cbui;
				cbui.isAlpha = uiInfo->isAlpha;
				cbui.alpha = uiInfo->alpha;

				_UI_PS->ConstantBufferUpdate(&cbui, "cbUI");
				_UI_PS->SetResourceViewBuffer(uiInfo->panelImage, "gDiffuseMap");
				_UI_PS->Update();

				// 어떻게 그릴지.. ui는 solid?
				g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidClass()->GetRasterizerState().Get());

				unsigned int stride = ResourceManager::GetInstance()->GetMesh((int)MeshName::ScreenMesh)->stride;
				unsigned int offset = 0;

				// 어떤 방식으로 그릴지
				g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh((int)MeshName::ScreenMesh)->GetPrimitiveTopology());

				// 버텍스 버퍼 설정
				g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh((int)MeshName::ScreenMesh)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

				// 인덱스 버퍼 설정
				g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh((int)MeshName::ScreenMesh)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

				// 그린다!
				g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh((int)MeshName::ScreenMesh)->GetIdxBufferSize(), 0, 0);
			}
			break;
			default:
				break;
		}
	}
}

void UIPass::EndRender()
{
	
	g_deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

