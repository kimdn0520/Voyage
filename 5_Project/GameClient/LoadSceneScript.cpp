#include "pch.h"
#include "LoadSceneScript.h"
#include "GameObject.h"
#include "GraphicsEngineManager.h"
#include "SceneManager.h"

LoadSceneScript::LoadSceneScript(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject)
{
	_nextScene = SceneManager::GetInstance()->GetLoadSceneName();
}

LoadSceneScript::~LoadSceneScript()
{}

void LoadSceneScript::Update()
{	
	SceneManager::GetInstance()->LoadScene(_nextScene);
}
