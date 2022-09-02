#include "pch.h"
#include "GraphicsEngineAPI.h"
#include "GraphicsEngine.h"
#include "VertexDefine.h"
#include "ResourceManager.h"

shared_ptr<GraphicsEngineAPI> GraphicsEngineAPI::graphicsEngine = nullptr;

// GraphicsEngineAPI을 상속받고있는 GraphicsEngine의 shared객체 생성
// 뉴비엔진의 그래픽 매니저로 넘겨줄 것이다.
shared_ptr<GraphicsEngineAPI> GraphicsEngineAPI::GetInstance()
{
	if(graphicsEngine == nullptr)
		graphicsEngine = make_shared<GraphicsEngine>();

	return graphicsEngine;
}