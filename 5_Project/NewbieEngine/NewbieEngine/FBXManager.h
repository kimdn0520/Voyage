#pragma once
#include "NewbieEngineDLL.h"

class FBXManager
{
public:
	FBXManager() {};
	~FBXManager() {};

private:
	static shared_ptr<FBXManager> fbxManager;

public:
	static NewbieEngine_DLL shared_ptr<FBXManager> GetInstance();

};

