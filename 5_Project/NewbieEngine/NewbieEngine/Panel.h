#pragma once
#include "NewbieEngineDLL.h"
#include "UI.h"
#include "ObjectInfo.h"

class Panel : public UI
{
public:
	NewbieEngine_DLL Panel(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~Panel();

private:
	float _left_Offset;
	float _right_Offset;
	float _top_Offset;
	float _bottom_Offset;


	// 하나만 쓰겠지?
	wstring _image;

	shared_ptr<UIInfo> _UIInfo;

public:
	Vector2 screenResize;

public:
	NewbieEngine_DLL void SetIsAlpha(bool value) { _UIInfo->isAlpha = value; }

	NewbieEngine_DLL const float& GetAlpha() { return _UIInfo->alpha; }

	NewbieEngine_DLL void SetAlpha(float alpha);

	NewbieEngine_DLL void SetOffset(float left, float right, float top, float bottom);

	NewbieEngine_DLL void SetImages(wstring name) { _image = name; }

	void Resize(int width, int height) override;

	void Update() override;

	void Render() override;
};

