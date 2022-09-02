#include "pch.h"
#include "HappyEndingSceneScript.h"
#include "GameObject.h"
#include "GraphicsEngineManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Panel.h"
#include "EventMachine.h"

HappyEndingSceneScript::HappyEndingSceneScript(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject)
{}

HappyEndingSceneScript::~HappyEndingSceneScript()
{}

void HappyEndingSceneScript::Update()
{
	_curTime += TimeManager::GetInstance()->GetDeltaTime();

	if (_curTime > 5.f && _curTime <= 10.f)
	{
		GetGameObject()->GetComponent<Panel>()->SetImages(L"Happy ending scene 2.png");
	}
	else if (_curTime > 10.f && _curTime <= 15.f)
	{
		GetGameObject()->GetComponent<Panel>()->SetImages(L"Happy ending scene 3.png");
	}
	else if (_curTime > 15.f && _curTime <= 20.f)
	{
		GetGameObject()->GetComponent<Panel>()->SetImages(L"Happy ending scene 4.png");
	}
	else if (_curTime > 25.f && _curTime <= 30.f)
	{
		GetGameObject()->GetComponent<Panel>()->SetImages(L"ex3.png");
	}
	else if (_curTime > 35.f && _curTime <= 40.f)
	{
		GetGameObject()->GetComponent<Panel>()->SetImages(L"ex4.png");
	}
	else if (_curTime > 45.f)
	{
		EventMachine::GetInstance()->ResetMachine();
		SceneManager::GetInstance()->LoadScene("TitleScene");
	}
}
