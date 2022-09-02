#pragma once
#include "ObjectInfo.h"
#include "GraphicsEngineAPI.h"
#include "ParserData.h"

class GraphicsEnigineManager
{
public:
	GraphicsEnigineManager() {};
	~GraphicsEnigineManager() {};

private:
	static shared_ptr<GraphicsEngineAPI> graphicsEngine;

public:
	static shared_ptr<GraphicsEngineAPI> GetInstance();

public:
	const WindowInfo& GetWindowInfo();

public:
	void Init(HWND hwnd, int width, int height);
};
