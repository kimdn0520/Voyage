#include "pch.h"
#include "GraphicsEngineAPI.h"
#include "GraphicsEngineManager.h"
#include "NewbieEngineFunc.h"
#include "NewbieEngine.h"
#include "SceneManager.h"
#include "AudioEngineDLL.h"
#include "Scene.h"
#include "UI.h"
#include "Text.h"

unique_ptr<NewbieEngine> GEngine; 

void EngineInit(WindowInfo windowInfo)
{
	// ���ӿ��� ����
	GEngine = make_unique<NewbieEngine>();

	// �׷��Ƚ� ����?
	GraphicsEngineManager::GetInstance()->Init(windowInfo.hwnd, windowInfo.width, windowInfo.height);

	// �״��� ���ӿ���?
	GEngine->Init(windowInfo);

	GraphicsEngineManager::GetInstance()->GetWindowInfo();
}          

void EngineUpdate()
{
	// ���ӿ��� ������Ʈ �ȿ� ������ ���ư�(�׷��Ƚ��� ����ť�� �־��ش�.)
	GEngine->Update();

	// ���ӿ��� ����(�׷��Ƚ��� ����ť�� �־��ش�.)
	//GEngine->Render();
}

/// <summary>
/// �׷��Ƚ��� Render �Լ��� ���������� Draw�� ���ִ� �κ��̴�.
/// </summary>
void EngineRender()
{
	GraphicsEngineManager::GetInstance()->BeginRender();

	GraphicsEngineManager::GetInstance()->Render();

	GraphicsEngineManager::GetInstance()->EndRender();
}

void EngineShutdown()
{
	GraphicsEngineManager::GetInstance()->Release();

	SceneManager::GetInstance()->Release();

	GEngine->Release();

	GEngine.release();
}

void ResizeWindow(int width, int height)
{
	if (GEngine != nullptr)
	{
		GraphicsEngineManager::GetInstance()->OnResize(width, height);
		
		auto UIs = SceneManager::GetInstance()->GetActiveScene()->GetUI();

		auto Texts = SceneManager::GetInstance()->GetActiveScene()->GetText();

		// ui������Ʈ�� width, height�� �������ش�.
		for (auto& ui : UIs)
		{
			ui->Resize(width, height);
		}

		for (auto& text : Texts)
		{
			text->Resize(width, height);
		}
	}
}
