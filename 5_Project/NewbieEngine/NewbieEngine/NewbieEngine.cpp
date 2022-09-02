#include "pch.h"
#include "GraphicsEngineAPI.h"
#include "NewbieEngine.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Resources.h"
#include "AudioEngineDLL.h"

void NewbieEngine::Init(WindowInfo windowInfo)
{
	InputManager::GetInstance()->Init(windowInfo.hwnd);
	TimeManager::GetInstance()->Init();
	Resources::GetInstance()->Init();

	bool fuck = DLLAudio::Initialize("Data/FmodBank");
	DLLAudio::LoadAllBank(Resources::GetInstance()->GetBankNames());
	DLLAudio::Set3DSetting(1.0f, 1.0f);
}


bool NewbieEngine::Update()
{
	InputManager::GetInstance()->Update();
	TimeManager::GetInstance()->Update();

	// 생성 에약된 오브젝트가 있다면 현재 씬에 생성해준다.
	SceneManager::GetInstance()->ReservedInstantiateGameObject();

	// 제거 예약된 오브젝트가 있다면 현재 씬에서 제거해준다.
	SceneManager::GetInstance()->ReservedRemoveGameObject();

	// 예약된 로드씬이 있다면 불러온다.
	if (SceneManager::GetInstance()->isReservedLoadScene)
	{
		SceneManager::GetInstance()->ReservedSceneLoad();
	}
	else
	{
		SceneManager::GetInstance()->Update();
		Render();
	}


	DLLAudio::Update();
	return true;
}

void NewbieEngine::Render()
{
	SceneManager::GetInstance()->Render();
}

void NewbieEngine::Release()
{
	DLLAudio::Release();
}

