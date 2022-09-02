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
	// 게임엔진 생성
	GEngine = make_unique<NewbieEngine>();

	// 그래픽스 부터?
	GraphicsEngineManager::GetInstance()->Init(windowInfo.hwnd, windowInfo.width, windowInfo.height);

	// 그다음 게임엔진?
	GEngine->Init(windowInfo);

	GraphicsEngineManager::GetInstance()->GetWindowInfo();
}          

void EngineUpdate()
{
	// 게임엔진 업데이트 안에 렌더도 돌아감(그래픽스의 렌더큐에 넣어준다.)
	GEngine->Update();

	// 게임엔진 렌더(그래픽스의 렌더큐에 넣어준다.)
	//GEngine->Render();
}

/// <summary>
/// 그래픽스의 Render 함수들 실질적으로 Draw를 해주는 부분이다.
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

		// ui오브젝트의 width, height도 조정해준다.
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
