#pragma once
#include "NewbieEngineDLL.h"

enum class KEY_TYPE
{
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',

	Q = 'Q',
	E = 'E',
	Z = 'Z',
	C = 'C',

	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',

	F1 = VK_F1,
	F3 = VK_F3,
	F4 = VK_F4,

	LBUTTON = VK_LBUTTON,
	RBUTTON = VK_RBUTTON,

	ESCAPE = VK_ESCAPE,
	SPACE = VK_SPACE,
};

enum class KEY_STATE
{
	NONE,
	PRESS,
	DOWN,
	UP,
	END
};

enum
{
	KEY_TYPE_COUNT = static_cast<__int32>(UINT8_MAX + 1),
	KEY_STATE_COUNT = static_cast<__int32>(KEY_STATE::END),
};

class InputManager
{
public:
	InputManager() {};
	~InputManager() {};

private:
	static shared_ptr<InputManager> inputManager;

private:
	bool _isAnykey = false;

public:
	static NewbieEngine_DLL shared_ptr<InputManager> GetInstance();

public:
	void Init(HWND hwnd);
	void Update();

	NewbieEngine_DLL bool& GetIsAnyKey() { return _isAnykey; }

	// ������ ���� ��
	NewbieEngine_DLL bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
	// �� ó�� ������ ��
	NewbieEngine_DLL bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
	// �� ó�� ������ ���� ��
	NewbieEngine_DLL bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }

	NewbieEngine_DLL const POINT& GetMousePos() { return _mousePos; }

	NewbieEngine_DLL float GetAxisRawHorizontal();
	NewbieEngine_DLL float GetAxisRawVertical();

private:
	inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<unsigned __int8>(key)]; }

private:
	HWND _hwnd;
	vector<KEY_STATE> _states;
	POINT _mousePos = {};
};

