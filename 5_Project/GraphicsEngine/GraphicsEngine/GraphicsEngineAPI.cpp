#include "pch.h"
#include "GraphicsEngineAPI.h"
#include "GraphicsEngine.h"
#include "VertexDefine.h"
#include "ResourceManager.h"

shared_ptr<GraphicsEngineAPI> GraphicsEngineAPI::graphicsEngine = nullptr;

// GraphicsEngineAPI�� ��ӹް��ִ� GraphicsEngine�� shared��ü ����
// �������� �׷��� �Ŵ����� �Ѱ��� ���̴�.
shared_ptr<GraphicsEngineAPI> GraphicsEngineAPI::GetInstance()
{
	if(graphicsEngine == nullptr)
		graphicsEngine = make_shared<GraphicsEngine>();

	return graphicsEngine;
}