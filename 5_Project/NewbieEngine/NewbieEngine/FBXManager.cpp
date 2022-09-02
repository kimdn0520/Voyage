#include "pch.h"
#include "FBXManager.h"

shared_ptr<FBXManager> FBXManager::fbxManager = nullptr;

shared_ptr<FBXManager> FBXManager::GetInstance()
{
	if (fbxManager == nullptr)
		fbxManager = make_shared<FBXManager>();

	return fbxManager;
}
