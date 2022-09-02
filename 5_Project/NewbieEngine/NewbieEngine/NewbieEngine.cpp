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

	// ���� ����� ������Ʈ�� �ִٸ� ���� ���� �������ش�.
	SceneManager::GetInstance()->ReservedInstantiateGameObject();

	// ���� ����� ������Ʈ�� �ִٸ� ���� ������ �������ش�.
	SceneManager::GetInstance()->ReservedRemoveGameObject();

	// ����� �ε���� �ִٸ� �ҷ��´�.
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

