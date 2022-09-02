#include "pch.h"
#include "Text.h"
#include "GameObject.h"
#include "GraphicsEngineManager.h"

Text::Text(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::TEXT),
	_textInfo(make_shared<TextInfo>())
{
	screenResize.x = 1920.f;
	screenResize.y = 1080.f;

	_textInfo->scale = 1.0f;		// 일단 기본 크기 1.0f으로 해놓았습니다.
}

Text::~Text()
{}

void Text::SetLocation(int x, int y, float screenSizeX, float screenSizeY)
{
	float ratio_x = screenResize.x / screenSizeX;
	float ratio_y = screenResize.y / screenSizeY;

	_x = x;
	_y = y;

	_x *= ratio_x;
	_y *= ratio_y;
}

void Text::Resize(int width, int height)
{
	float ratio_x = (float)width / screenResize.x;
	float ratio_y = (float)height / screenResize.y;

	_x *= ratio_x;
	_y *= ratio_y;

	screenResize.x = width;
	screenResize.y = height;
}

void Text::Render()
{
	_textInfo->x = _x;
	_textInfo->y = _y;
	GraphicsEngineManager::GetInstance()->SetTextInfo(_textInfo);
}
