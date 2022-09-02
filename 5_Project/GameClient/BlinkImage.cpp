#include "pch.h"
#include "BlinkImage.h"
#include "GameObject.h"
#include "Transform.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Panel.h"
#include "GraphicsEngineManager.h"
#include "SceneManager.h"
#include "SoundEvent.h"
#include "SoundManager.h"

BlinkImage::BlinkImage(shared_ptr<GameObject> gameObject)
	: MonoBehaviour(gameObject), _alpha(0.f), _blinkTime(1.f)
{
	_panel = gameObject->GetComponent<Panel>();
}

BlinkImage::~BlinkImage()
{}

void BlinkImage::Update()
{
	if (_isFade)
	{
		_alpha += TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha >= 1.0f)
		{
			_isFade = false;
		}
	}
	else
	{
		_alpha -= TimeManager::GetInstance()->GetDeltaTime() / _blinkTime;

		if (_alpha <= 0.f)
		{
			_isFade = true;
		}
	}

	_panel->SetAlpha(_alpha);
}
