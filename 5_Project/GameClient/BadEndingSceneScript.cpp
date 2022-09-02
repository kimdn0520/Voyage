#include "pch.h"
#include "BadEndingSceneScript.h"
#include "GameObject.h"
#include "GraphicsEngineManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Panel.h"
#include "EventMachine.h"

BadEndingSceneScript::BadEndingSceneScript(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject)
{}

BadEndingSceneScript::~BadEndingSceneScript()
{}

void BadEndingSceneScript::Update()
{
	_curTime += TimeManager::GetInstance()->GetDeltaTime();

	if (_curTime >= 3.f && _curTime <= 8.f)
	{
		GetGameObject()->GetComponent<Panel>()->SetImages(L"ex3.png");
	}
	else if (_curTime >= 9.f && _curTime <= 13.f)
	{
		GetGameObject()->GetComponent<Panel>()->SetImages(L"ex4.png");
	}
	else if (_curTime >= 14.f)
	{
		SceneManager::GetInstance()->LoadScene("TitleScene");
		EventMachine::GetInstance()->ResetMachine();
	}
}
