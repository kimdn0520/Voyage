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


	// �̹����� ��� �������? -> ������ �׷��Ƚ�����
	vector<wstring> _images;

	shared_ptr<UIInfo> _UIInfo;
public:
	Vector2 screenResize;

public:
	NewbieEngine_DLL void SetIsAlpha(bool value) { _UIInfo->isAlpha = value; }

	NewbieEngine_DLL const float& GetAlpha() { return _UIInfo->alpha; }

	NewbieEngine_DLL void SetAlpha(float alpha);

	// ���� ��ư�� ���¸� ���ɴϴ�.
	NewbieEngine_DLL ButtonStatus GetStatus() { return _status; }

	// ��ư�� ��ġ�� �����մϴ�. �������� (0, 0) ���� ���� �����ϸ� �˴ϴ�.
	NewbieEngine_DLL void SetOffset(float left, float right, float top, float bottom);

	// �̹����� �ֽ��ϴ�. 0 : normal / 1 : highlight / 2 : pressed 
	NewbieEngine_DLL void SetImages(int idx , wstring name);

	void Resize(int width, int height) override;

	void Update() override;

	void Render() override;
};

