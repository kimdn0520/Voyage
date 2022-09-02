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
	return  graphicsEngine->GetWindowInfo();  	// ���ӿ����� �ִ� windowinfo�� �׷��Ƚ� ����ü�� ����?
}

void GraphicsEngineManager::Init(HWND hwnd, int width, int height)
{
	// ������ �׷��Ƚ� ���� shared��ü�� ���´�.
	graphicsEngine = GraphicsEngineAPI::GetInstance();

	graphicsEngine->Init(hwnd, width, height);
}
