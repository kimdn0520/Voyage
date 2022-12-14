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

// ???????? ?????? ????
void RenderManager::DeferredRender()
{
	ReflectionRender();

	// Bloom?? ???? ???? Bloom?? ???????? ?????????? ??????. 
	//_PreBloomPass->BeginRender();		Render???? ???????? ????. (??????????)
	_PreBloomPass->Render(_renderData);
	_BloomPass->BeginRender();
	// Bloom?? ?????????? ?????????? ????????.
	_BloomPass->SetBloomSRV(_PreBloomPass->GetBloomObjectsSRV());

	// ???????? ???? ?????????? ????????.
	_deferredPass->BeginRender();

	// ???? ???????? ????????. (?????????? ?????? ?????? ???? ??????.)
	// ?????? ???????????? ?????????? ?? ???? ???? ???????? ?????? ?????? ?????? ?????? ??????.
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
	// Bloom?? ????
	//_BloomPass->Render(_renderData);		// RenderData?? ?????????? bool?? ???????????????? ???? ????.
	//_BloomPass->EndRender();

}
