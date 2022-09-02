#pragma once
#include "NewbieEngineDLL.h"
#include "Component.h"
#include "ObjectInfo.h"

class Text : public Component
{
public:
	NewbieEngine_DLL Text(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~Text();

private:
	shared_ptr<TextInfo> _textInfo;


	int _x = 0;
	int _y = 0;
public:
	Vector2 screenResize;

public:
	NewbieEngine_DLL shared_ptr<TextInfo> GetTextInfo() { return _textInfo; }

	NewbieEngine_DLL void SetLocation(int x, int y, float screenSizeX, float screenSizeY);

public:
	NewbieEngine_DLL void Resize(int width, int height);

	virtual void Render() override;
};

