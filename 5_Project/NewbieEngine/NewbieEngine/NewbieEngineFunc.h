#pragma once
#include "NewbieEngineDLL.h"

extern "C" NewbieEngine_DLL void EngineInit(WindowInfo windowInfo);
extern "C" NewbieEngine_DLL void EngineUpdate();
extern "C" NewbieEngine_DLL void EngineRender();
extern "C" NewbieEngine_DLL void EngineShutdown();
extern "C" NewbieEngine_DLL void ResizeWindow(int width, int height);

