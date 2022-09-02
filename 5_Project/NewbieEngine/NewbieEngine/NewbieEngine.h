#pragma once
#include "NewbieEngineDLL.h"

class NewbieEngine
{
private:
	//WindowInfo _windowInfo;

public:
	//WindowInfo& GetWindow() { return _windowInfo; }

	void Init(WindowInfo windowInfo);
	bool Update();
	void Render();
	void Release();
};

