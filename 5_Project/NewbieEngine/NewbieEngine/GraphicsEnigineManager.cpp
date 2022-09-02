#include "pch.h"
#include "GraphicsEnigineManager.h"

shared_ptr<GraphicsEngineAPI> GraphicsEnigineManager::graphicsEngine = nullptr;

shared_ptr<GraphicsEngineAPI> GraphicsEnigineManager::GetInstance()
{
	if (graphicsEngine == nullptr)
		graphicsEngine = GraphicsEngineAPI::GetInstance();

	return graphicsEngine;
}

const WindowInfo& GraphicsEnigineManager::GetWindowInfo()
{
	return  graphicsEngine->GetWindowInfo();  	// ���ӿ����� �ִ� windowinfo�� �׷��Ƚ� ����ü�� ����?
}

void GraphicsEnigineManager::Init(HWND hwnd, int width, int height)
{
	// ������ �׷��Ƚ� ���� shared��ü�� ���´�.
	graphicsEngine = GraphicsEngineAPI::GetInstance();

	graphicsEngine->Init(hwnd, width, height);
}
