#include "pch.h"
#include "InputManager.h"
#include "GraphicsEngineManager.h"
#include "GraphicsEngineAPI.h"

shared_ptr<InputManager> InputManager::inputManager = nullptr;

shared_ptr<InputManager> InputManager::GetInstance()
{
	if (inputManager == nullptr)
		inputManager = make_shared<InputManager>();

	return inputManager;
}

void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

void InputManager::Update()
{
	HWND hwnd = ::GetActiveWindow();
	if (_hwnd != hwnd)
	{
		for (unsigned __int64 key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;

		return;
	}

	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (unsigned __int64 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		if(key == 25)
			continue;

		// Ű�� ���� ������ true
		if (asciiKeys[key] & 0x80)
		{
			KEY_STATE& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
			{
				_isAnykey = true; 
				state = KEY_STATE::DOWN;
			}
		}
		else
		{
			KEY_STATE& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
			{
				_isAnykey = false;
				state = KEY_STATE::UP;
			}
			else
				state = KEY_STATE::NONE;
		}
	}

	::GetCursorPos(&_mousePos);
	::ScreenToClient(GraphicsEngineManager::GetInstance()->GetWindowInfo().hwnd, &_mousePos);
}

float InputManager::GetAxisRawHorizontal()
{
	if (GetButton(KEY_TYPE::LEFT))
		return -1.f;
	else if (GetButton(KEY_TYPE::RIGHT))
		return 1.f;

	if (GetButtonUp(KEY_TYPE::LEFT))
		return 0.f;
	else if (GetButtonUp(KEY_TYPE::RIGHT))
		return 0.f;

	return 0.f;
}

float InputManager::GetAxisRawVertical()
{
	if (GetButton(KEY_TYPE::UP))
		return 1.f;
	else if (GetButton(KEY_TYPE::DOWN))
		return -1.f;

	if (GetButtonUp(KEY_TYPE::UP))
		return 0.f;
	else if (GetButtonUp(KEY_TYPE::DOWN))
		return 0.f;

	return 0.f;
}