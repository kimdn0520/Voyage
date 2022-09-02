#include "pch.h"
#include "PressAnyKey.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Panel.h"
#include "GraphicsEngineManager.h"
#include "SceneManager.h"

PressAnyKey::PressAnyKey(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject), _alpha(0.f), _blinkTime(2.f), _isFade(false)
{
	_panel = gameObject->GetComponent<Panel>();
}

PressAnyKey::~PressAnyKey()
{}

void PressAnyKey::Update()
{
	if(_isFade)
		_blinkTime += TimeManager::GetInstance()->GetDeltaTime();
	
	if (InputManager::GetInstance()->GetIsAnyKey())
	{
		_isFade = true;
	}

	if (_isFade)
	{
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha >= 1.0f)
		{
			_isFade = false;
			_panel->SetIsAlpha(_isFade);

			// 다음에 로드할 씬이름을 Set해주고
			SceneManager::GetInstance()->SetLoadSceneName("Infancy");

			// 로딩씬으로 넘어간다.
			SceneManager::GetInstance()->LoadScene("LoadingScene");
		}

		_panel->SetAlpha(_alpha);
	}
}
