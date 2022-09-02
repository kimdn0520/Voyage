#include "pch.h"
#include "Button.h"
#include "InputManager.h"
#include "GraphicsEngineManager.h"
#include "Transform.h"
#include "GameObject.h"

Button::Button(shared_ptr<GameObject> gameObject)
	: UI(gameObject),
	  _left_Offset(0.f), _right_Offset(0.f), _top_Offset(0.f), _bottom_Offset(0.f),
	 _UIInfo(make_shared<UIInfo>()), _transform(gameObject->GetComponent<Transform>())
{
	screenResize.x = 1920.f;
	screenResize.y = 1080.f;

	_images.resize(3, L"");	// NORMAL, HIGHLIGHT, PRESSED 3가지만 하면 될듯함;
}

Button::~Button()
{}

void Button::SetAlpha(float alpha)
{
	_UIInfo->alpha = alpha;
}

void Button::SetOffset(float left, float right, float top, float bottom)
{
	_left_Offset = left;
	_right_Offset = right;
	_top_Offset = top;
	_bottom_Offset = bottom;
}

void Button::SetImages(int idx, wstring name)
{
	_images[idx] = name; 
}

/// <summary>
/// 화면크기가 바뀌면 버튼 크기랑 위치(offset) 조정해줘야함
/// </summary>
void Button::Resize(int width, int height)
{
	float ratio_x = (float)width / screenResize.x;
	float ratio_y = (float)height / screenResize.y;

	_left_Offset *= ratio_x;
	_right_Offset *= ratio_x;
	_top_Offset *= ratio_y;
	_bottom_Offset *= ratio_y;

	screenResize.x = width;
	screenResize.y = height;
}

void Button::Update()
{
	const POINT& mousePoint = InputManager::GetInstance()->GetMousePos();

	if (_left_Offset <= mousePoint.x && _right_Offset >= mousePoint.x)
	{
		if (_top_Offset <= mousePoint.y && _bottom_Offset >= mousePoint.y)
		{
			// 마우스 왼쪽 눌렀을때
			if (InputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON) || InputManager::GetInstance()->GetButton(KEY_TYPE::LBUTTON))
			{
				_status = ButtonStatus::PRESSED;
			}
			// 마우스 왼쪽 누르고 땠을때
			else if (InputManager::GetInstance()->GetButtonUp(KEY_TYPE::LBUTTON))
			{
				_status = ButtonStatus::PRESSEDUP;
			}
			// 그냥 마우스가 버튼 위에 있을때
			else
			{
				_status = ButtonStatus::HIGHLIGHT;
			}
		}
		else
		{
			_status = ButtonStatus::NORMAL;
		}
	}
	else
	{
		_status = ButtonStatus::NORMAL;
	}
}

void Button::Render()
{
	_UIInfo->uiType = UIType::Button;
	_UIInfo->sortLayer = _sortLayer;
	_UIInfo->left_Offset = _left_Offset;
	_UIInfo->right_Offset = _right_Offset;
	_UIInfo->top_Offset = _top_Offset;
	_UIInfo->bottom_Offset = _bottom_Offset;

	_UIInfo->width = screenResize.x;
	_UIInfo->height = screenResize.y;
	_UIInfo->status = _status;
	_UIInfo->normalImage = _images[(int)ButtonStatus::NORMAL];
	_UIInfo->highLightImage = _images[(int)ButtonStatus::HIGHLIGHT];
	_UIInfo->pressedImage = _images[(int)ButtonStatus::PRESSED];

	GraphicsEngineManager::GetInstance()->SetUIInfo(_UIInfo);
}