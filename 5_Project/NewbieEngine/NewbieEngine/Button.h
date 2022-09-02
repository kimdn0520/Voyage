#pragma once
#include "NewbieEngineDLL.h"
#include "UI.h"
#include "ObjectInfo.h"

class Transform;

class Button : public UI
{
public:
	NewbieEngine_DLL Button(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~Button();

private:
	shared_ptr<Transform> _transform;

	float _left_Offset;
	float _right_Offset;
	float _top_Offset;
	float _bottom_Offset;

	ButtonStatus _status = ButtonStatus::NORMAL;


	// 이미지를 어떻게 넣을까요? -> 생성은 그래픽스에서
	vector<wstring> _images;

	shared_ptr<UIInfo> _UIInfo;
public:
	Vector2 screenResize;

public:
	NewbieEngine_DLL void SetIsAlpha(bool value) { _UIInfo->isAlpha = value; }

	NewbieEngine_DLL const float& GetAlpha() { return _UIInfo->alpha; }

	NewbieEngine_DLL void SetAlpha(float alpha);

	// 현재 버튼의 상태를 얻어옵니다.
	NewbieEngine_DLL ButtonStatus GetStatus() { return _status; }

	// 버튼의 위치를 설정합니다. 왼쪽위를 (0, 0) 으로 보고 설정하면 됩니다.
	NewbieEngine_DLL void SetOffset(float left, float right, float top, float bottom);

	// 이미지를 넣습니다. 0 : normal / 1 : highlight / 2 : pressed 
	NewbieEngine_DLL void SetImages(int idx , wstring name);

	void Resize(int width, int height) override;

	void Update() override;

	void Render() override;
};

