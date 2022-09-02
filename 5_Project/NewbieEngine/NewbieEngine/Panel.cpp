#include "pch.h"
#include "Panel.h"
#include "InputManager.h"
#include "GraphicsEngineManager.h"

Panel::Panel(shared_ptr<GameObject> gameObject)
	: UI(gameObject),
	_left_Offset(0.f), _right_Offset(0.f), _top_Offset(0.f), _bottom_Offset(0.f),
	_UIInfo(make_shared<UIInfo>())
{
	screenResize.x = 1920.f;
	screenResize.y = 1080.f;

	_UIInfo->isAlpha = false;
	_UIInfo->alpha = 1.0f;
}

Panel::~Panel()
{}

void Panel::SetAlpha(float alpha)
{
	_UIInfo->alpha = alpha;
}

void Panel::SetOffset(float left, float right, float top, float bottom)
{
	_left_Offset = left;
	_right_Offset = right;
	_top_Offset = top;
	_bottom_Offset = bottom;
}

void Panel::Resize(int width, int height)
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

void Panel::Update()
{
	//const POINT& mousePoint = InputManager::GetInstance()->GetMousePos();

	//if (_left_Offset <= mousePoint.x && _right_Offset >= mousePoint.x)
	//{
	//	if (_top_Offset <= mousePoint.y && _bottom_Offset >= mousePoint.y)
	//	{
	//		// 흠 패널을 누르게될경우..?가있나 일단 스킾
	//	}
	//}
}

void Panel::Render()
{
	_UIInfo->uiType = UIType::Panel;
	_UIInfo->sortLayer = _sortLayer;
	_UIInfo->left_Offset = _left_Offset;
	_UIInfo->right_Offset = _right_Offset;
	_UIInfo->top_Offset = _top_Offset;
	_UIInfo->bottom_Offset = _bottom_Offset;

	_UIInfo->width = screenResize.x;
	_UIInfo->height = screenResize.y;
	_UIInfo->panelImage = _image;

	GraphicsEngineManager::GetInstance()->SetUIInfo(_UIInfo);
}
