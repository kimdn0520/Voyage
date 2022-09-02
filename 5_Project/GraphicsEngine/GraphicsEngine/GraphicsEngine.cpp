#include "pch.h"
#include "GraphicsEngine.h"
#include "Device.h"
#include "SwapChain.h"
#include "RenderTargetView.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "DDSTextureLoader.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "PassBase.h"
#include "LightManager.h"
#include "VertexDefine.h"
#include "DebugWindow.h"
#include "RenderManager.h"
#include "BlendState.h"
#include "UIPass.h"
#include "OceanTestPass.h"

WindowInfo GraphicsEngine::windowInfo;		// ������ ����
bool GraphicsEngine::_enable4xMsaa = true;	// ��Ƽ���ø� ��뿩��

void GraphicsEngine::TurnZBufferOn()
{
	_deviceClass->GetDeviceContext()->OMSetDepthStencilState(_depthStencilStateClass->GetDepthStencilState().Get(), 1);
}

void GraphicsEngine::TurnZBufferOff()
{
	_deviceClass->GetDeviceContext()->OMSetDepthStencilState(_disableDepthStencilStateClass->GetDepthStencilState().Get(), 1);
}

void GraphicsEngine::TurnSkyboxOn()
{
	_deviceClass->GetDeviceContext()->OMSetDepthStencilState(_depthStencilStateClassSkybox->GetDepthStencilState().Get(), 1);
}

void GraphicsEngine::Init(HWND hwnd, int width, int height)
{
	windowInfo.hwnd = hwnd;
	windowInfo.width = width;
	windowInfo.height = height;

	// ���� ������ �ʿ��� Ŭ������ �ʱ�ȭ
	_deviceClass = make_shared<Device>();
	_swapChainClass = make_shared<SwapChain>();
	_mainRenderTargetViewClass = make_shared<RenderTargetView>();
	_depthStencilStateClass = make_shared<DepthStencilState>();
	_disableDepthStencilStateClass = make_shared<DepthStencilState>();
	_depthStencilStateClassSkybox = make_shared<DepthStencilState>();
	_solidRasterizerStateClass = make_shared<RasterizerState>();
	_wireRasterizerStateClass = make_shared<RasterizerState>();
	_solidNoneCullRasterizerStateClass = make_shared<RasterizerState>();
	_alphaBlendStateClass = make_shared<BlendState>();

	// Init
	_deviceClass->Init();
	_swapChainClass->Init(_deviceClass);
	_solidRasterizerStateClass->Init(_deviceClass, D3D11_CULL_BACK ,D3D11_FILL_SOLID);
	_wireRasterizerStateClass->Init(_deviceClass, D3D11_CULL_BACK, D3D11_FILL_WIREFRAME);
	_solidNoneCullRasterizerStateClass->Init(_deviceClass, D3D11_CULL_NONE ,D3D11_FILL_SOLID);

	OnResize(width, height);

	// ��θ� Ŭ���̾�Ʈ �ʿ� ���� �ǳ�?
	// Shader ��δ� GameClient -> Data -> Shader �� �ֽ��ϴ�. 
	ShaderManager::GetInstance()->SetFilePath(L"Data/Shader/");
	ShaderManager::GetInstance()->CreateAllShaders();

	// �Ŵ������� Init �� �ʱ�ȭ ���ִ� �κ��Դϴ�.
	ResourceManager::GetInstance()->Init();
	LightManager::GetInstance()->Init();
	RenderManager::GetInstance()->Init();
}

void GraphicsEngine::OnResize(int width, int height)
{
	windowInfo.width = width;
	windowInfo.height = height;

	if (_deviceClass->GetDevice() != nullptr)
	{
		// ���ۿ� ���� ������ �����ϹǷ� ���� view, depth�� ����������Ѵ�!! 
		_mainRenderTargetViewClass->ResizeRelease();

		_mainRenderTargetViewClass->BackBufferRenderTarget(
			_deviceClass->GetDevice(),
			_deviceClass->GetDeviceContext().Get(),
			_swapChainClass->GetSwapChain().Get(),
			_deviceClass->Get4xMsaaQuality(),
			_enable4xMsaa,
			windowInfo.width,
			windowInfo.height
		);

		_depthStencilStateClass->Init(_deviceClass, true, D3D11_COMPARISON_LESS);
		_disableDepthStencilStateClass->Init(_deviceClass, false, D3D11_COMPARISON_LESS);
		_depthStencilStateClassSkybox->Init(_deviceClass, false, D3D11_COMPARISON_LESS_EQUAL);
		_alphaBlendStateClass->Init(_deviceClass);

		_deviceClass->GetDeviceContext()->OMSetDepthStencilState(_depthStencilStateClass->GetDepthStencilState().Get(), 1);
		
		if(RenderManager::GetInstance()->isInit)
			RenderManager::GetInstance()->OnResize(width, height);
	}
}

void GraphicsEngine::Release()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception. 
	if (_swapChainClass)
	{
		_swapChainClass->GetSwapChain()->SetFullscreenState(false, NULL);
	}
	
	// �Ŵ������� Release() ���ְ� �ֽ��ϴ�~
	ShaderManager::GetInstance()->Release();
	ResourceManager::GetInstance()->Release();
	LightManager::GetInstance()->ResetLightInfo();
	RenderManager::GetInstance()->Release();

	_swapChainClass->Release();
	_mainRenderTargetViewClass->Release();
	_depthStencilStateClass->Release();
	_disableDepthStencilStateClass->Release();
	_solidRasterizerStateClass->Release();
	_wireRasterizerStateClass->Release();
	_alphaBlendStateClass->Release();

	//OutputDebugStringA("-------���� ������Ʈ ����Դϴ�--------\r\n");
	//_deviceClass->GetDevice()->QueryInterface(_debug.GetAddressOf());
	//_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	//OutputDebugStringA("-------------------------------------\r\n");
	_deviceClass->Release();
}

void GraphicsEngine::BeginRender()
{
	float color[4];

	// Setup the color to clear the buffer to. 
	color[0] = 0.5f;
	color[1] = 0.5f;
	color[2] = 0.5f;
	color[3] = 0.5f;

	// Clear the back buffer.
	_deviceClass->GetDeviceContext()->ClearRenderTargetView(_mainRenderTargetViewClass->GetRenderTargetView().Get(), color);
	// Clear the depth buffer. 
	_deviceClass->GetDeviceContext()->ClearDepthStencilView(_mainRenderTargetViewClass->GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsEngine::Render()
{
	// ���� ���� ����Ʈ���� ������Ʈ ��Ű��
	LightManager::GetInstance()->Update();

	RenderManager::GetInstance()->Render();
}

void GraphicsEngine::EndRender()
{
	// Present the back buffer to the screen since rendering is complete. 
	if (_vsync_enabled)
	{
		// Lock to screen refresh rate. 60 ����
		_swapChainClass->GetSwapChain()->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.  ���� ����
		_swapChainClass->GetSwapChain()->Present(0, 0);
	}
}

void GraphicsEngine::SetObjInfo(shared_ptr<MeshInfo> meshInfo)
{
	// �����Ŵ����� �������Ϳ� �Ѱ��ٰ���
	RenderManager::GetInstance()->PushRenderData(meshInfo);
}

void GraphicsEngine::SetTextInfo(shared_ptr<TextInfo> textInfo)
{
	RenderManager::GetInstance()->PushTextData(textInfo);
}

void GraphicsEngine::SetCameraInfo(CameraInfo cameraInfo)
{
	RenderManager::s_cameraInfo = cameraInfo;
}

void GraphicsEngine::SetLightInfo(LightInfo lightInfo)
{
	LightManager::GetInstance()->SetLightInfo(lightInfo);
}

void GraphicsEngine::SetColDebugInfo(shared_ptr<ColDebugInfo> colDebugInfo)
{
	RenderManager::GetInstance()->PushColDebugData(colDebugInfo);
}

void GraphicsEngine::SetUIInfo(shared_ptr<UIInfo> uiInfo)
{
	RenderManager::GetInstance()->PushUIData(uiInfo);
}

void GraphicsEngine::UpdateLightInfo(LightInfo lightInfo)
{
	LightManager::GetInstance()->UpdateLightInfo(lightInfo);
}

void GraphicsEngine::SetMesh(vector<GraphicsVertex> vertices, vector<unsigned int> indicies, size_t name, int topology, int rasterState,bool isParticle)
{ 
	ResourceManager::GetInstance()->CreateMesh(vertices, indicies, name, topology, rasterState,isParticle);
}

void GraphicsEngine::SetBackBufferRenderTarget()
{
	//BeginRender();

	_deviceClass->GetDeviceContext()->OMSetRenderTargets(1, _mainRenderTargetViewClass->GetRenderTargetView().GetAddressOf(), _mainRenderTargetViewClass->GetDepthStencilView().Get());

	_deviceClass->GetDeviceContext()->RSSetViewports(1, &_mainRenderTargetViewClass->GetViewPort());
}

void GraphicsEngine::SetMaterialInfo(shared_ptr<MaterialInfo> materialInfo)
{
	ResourceManager::GetInstance()->materials.push_back(materialInfo);
}

void GraphicsEngine::RenewalMaterials()
{
	ResourceManager::GetInstance()->Renewal();
}

void GraphicsEngine::SetCubeMapTextureInfo(int materialID, wstring cubeMapTexture)
{
	ResourceManager::GetInstance()->materials[materialID]->cubeMapTexture = cubeMapTexture;
}

void GraphicsEngine::SetDiffuseMapTextureInfo(int materialID, wstring diffuseMapTexture)
{
	ResourceManager::GetInstance()->materials[materialID]->diffuseTexture = diffuseMapTexture;
}

void GraphicsEngine::SetNormalMapTextureInfo(int materialID, wstring normalMapTexture)
{
	ResourceManager::GetInstance()->materials[materialID]->normalTexture = normalMapTexture;
}

void GraphicsEngine::SetWaterCameraInfo(CameraInfo cameraInfo)
{
	RenderManager::s_waterCameraInfo.push_back(cameraInfo);
}

void GraphicsEngine::SetIsFade(bool value)
{
	UIPass::isFade = value;
}

void GraphicsEngine::SetAlpha(float alpha)
{
	UIPass::alpha = alpha;
}

void GraphicsEngine::ColDebugSwitch()
{
	RenderManager::GetInstance()->isColDebugSwitch ? RenderManager::GetInstance()->isColDebugSwitch = false : RenderManager::GetInstance()->isColDebugSwitch = true;
}

void GraphicsEngine::SetDeltaTime(float deltatime)
{
//	OceanTestPass::_waveTime = deltatime;
}

void GraphicsEngine::SetParticleDeletIndex(std::vector<int>& deleteindex)
{
	ResourceManager::GetInstance()->ReSetParticle(deleteindex);

}
