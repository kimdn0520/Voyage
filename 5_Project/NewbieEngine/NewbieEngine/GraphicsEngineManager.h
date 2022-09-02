#pragma once
#include "NewbieEngineDLL.h"
#include "ObjectInfo.h"
#include "GraphicsEngineAPI.h"
#include "ParserData.h"

class GraphicsEngineManager
{
public:
	NewbieEngine_DLL GraphicsEngineManager() {};
	NewbieEngine_DLL ~GraphicsEngineManager() {};

private:
	static shared_ptr<GraphicsEngineAPI> graphicsEngine;

public:
	NewbieEngine_DLL static shared_ptr<GraphicsEngineAPI> GetInstance();

public:
	const WindowInfo& GetWindowInfo();

public:
	void Init(HWND hwnd, int width, int height);
};
