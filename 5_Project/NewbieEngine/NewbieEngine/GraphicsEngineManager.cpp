#include "pch.h"
#include "GraphicsEngineManager.h"

shared_ptr<GraphicsEngineAPI> GraphicsEngineManager::graphicsEngine = nullptr;

shared_ptr<GraphicsEngineAPI> GraphicsEngineManager::GetInstance()
{
	if (graphicsEngine == nullptr)
		graphicsEngine = GraphicsEngineAPI::GetInstance();

	return graphicsEngine;
}

const WindowInfo& GraphicsEngineManager::GetWindowInfo()
{
	return  graphicsEngine->GetWindowInfo();  	// 게임엔진에 있는 windowinfo에 그래픽스 구조체가 들어가네?
}

void GraphicsEngineManager::Init(HWND hwnd, int width, int height)
{
	// 생성된 그래픽스 엔진 shared객체를 얻어온다.
	graphicsEngine = GraphicsEngineAPI::GetInstance();

	graphicsEngine->Init(hwnd, width, height);
}
