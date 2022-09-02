#include "pch.h"
#include "RenderManager.h"
#include "BasicPass.h"
#include "LightPass.h"
#include "DebugPass.h"
#include "TextPass.h"
#include "DeferredPass.h"
#include "ColDebugPass.h"
#include "UIPass.h"
#include "WaterPass.h"
#include "GraphicsEngineAPI.h"
#include "OceanTestPass.h"
#include "ReflectionSkyPass.h"
#include "ReflectionObjectPass.h"
#include "PreBloomPass.h"
#include "BloomPass.h"
#include "WaveEffectPass.h"

shared_ptr<RenderManager> RenderManager::renderManager = nullptr;
CameraInfo RenderManager::s_cameraInfo;
std::vector<CameraInfo> RenderManager::s_waterCameraInfo(6);

shared_ptr<RenderManager> RenderManager::GetInstance()
{
	if (renderManager == nullptr)
		renderManager = make_shared<RenderManager>();

	return renderManager;
}

void RenderManager::Init()
{
	isInit = true;

	_debugPass = make_shared<DebugPass>();
	_debugPass->Init();

	_textPass = make_shared<TextPass>();
	_textPass->Init();

	_deferredPass = make_shared<DeferredPass>();
	_deferredPass->Init();

	_colDebugPass = make_shared<ColDebugPass>();
	_colDebugPass->Init();

	_UIPass = make_shared<UIPass>();
	_UIPass->Init();

	_WaterPass = make_shared<WaterPass>();
	_WaterPass->Init();

	_OceanPass = make_shared<OceanTestPass>();
	_OceanPass->Init();

	_ReflectionSkyPass = make_shared<ReflectionSkyPass>();
	_ReflectionSkyPass->Init();

	_ReflectionObjectPass = make_shared<ReflectionObjectPass>();
	_ReflectionObjectPass->Init();

	_PreBloomPass = make_shared<PreBloomPass>();
	_PreBloomPass->Init();

	_BloomPass = make_shared<BloomPass>();
	_BloomPass->Init();

	_WavePass = make_shared<WaveEffectPass>();
	_WavePass->Init();
}

void RenderManager::OnResize(int width, int height)
{
	//_WaterPass->OnResize(width, height);
	_deferredPass->OnResize(width, height);
}

void RenderManager::Release()
{
	_renderData.clear();
	_textData.clear();

	_WaterPass->Release();

	_deferredPass->Release();

	_debugPass->Release();

	//_colDebugPass->Release();
	_ReflectionSkyPass->Release();
	_ReflectionObjectPass->Release();

	_UIPass->Release();
	_PreBloomPass->Release();
	_BloomPass->Release();
}

void RenderManager::PushRenderData(shared_ptr<MeshInfo> meshInfo)
{
	 _renderData.push_back(meshInfo); 
	 int a = 0;
}

void RenderManager::Render()
{
	/*for (auto data : _renderData)
	{
		if (data->type != OBJECT_TYPE::WATER)
			WaterRender();
	}*/
	

	DeferredRender();

	_WavePass->BeginRender();
	_WavePass->Render(_renderData);
	_WavePass->EndRender();

	UIRender();

	TextRender();

	//ColDebugRender();

	//DebugRender();			// MRT

	_renderData.clear();
	_textData.clear();
	_colDebugData.clear();
	_uiData.clear();
	s_waterCameraInfo.clear();
}

void RenderManager::TextRender()
{
	_textPass->RenderText(_textData);
}

void RenderManager::DebugRender()
{
	_debugPass->BeginRender();

	_debugPass->Render(_renderData);

	_debugPass->EndRender();
}

// 전체적인 렌더를 담당
void RenderManager::DeferredRender()
{
	ReflectionRender();

	// Bloom을 하기 전에 Bloom을 하기위한 오브젝트를 그린다. 
	//_PreBloomPass->BeginRender();		Render에서 진행해야 한다. (어쩔수업슴)
	_PreBloomPass->Render(_renderData);
	_BloomPass->BeginRender();
	// Bloom할 오브젝트의 텍스쳐들을 넣어준다.
	_BloomPass->SetBloomSRV(_PreBloomPass->GetBloomObjectsSRV());

	// 디퍼드에 그릴 렌더타켓을 세팅한다.
	_deferredPass->BeginRender();

	// 반사 텍스쳐를 넣어준다. (오브젝트와 하늘은 별개로 넣게 만든다.)
	// 하늘은 흘러가야하고 오브젝트는 그 사물 밑에 고정되어 있어야 하므로 그렇게 만들어 줘야함.
	_OceanPass->SetReflectionObjectSRV(_ReflectionObjectPass->GetObjectSRV());
	_OceanPass->SetReflectionSkySRV(_ReflectionSkyPass->GetSkySRV());

	_deferredPass->Render(_renderData);


	_OceanPass->Render(_renderData);

	if(isColDebugSwitch)
		ColDebugRender();

	_deferredPass->EndRender();

	//PostProcessing();
}

void RenderManager::ColDebugRender()
{
	_colDebugPass->Render(_colDebugData);
}

void RenderManager::UIRender()
{
	_UIPass->BeginRender();

	_UIPass->Render(_uiData);

	_UIPass->EndRender();
}

void RenderManager::WaterRender()
{
	/*_WaterPass->BeginRender();

	_WaterPass->Render(_renderData);*/
}

void RenderManager::ReflectionRender()
{
	_ReflectionObjectPass->BeginRender();

	_ReflectionObjectPass->Render(_renderData);

	_ReflectionSkyPass->BeginRender();

	_ReflectionSkyPass->Render(_renderData);
}

void RenderManager::PostProcessing()
{
	// Bloom을 진행
	//_BloomPass->Render(_renderData);		// RenderData가 필요없으면 bool로 넘겨주는걸로해서 해도 된다.
	//_BloomPass->EndRender();

}
