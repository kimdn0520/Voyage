#pragma once
#include "NewbieEngineDLL.h"
#include "UI.h"

class Transform;

/// <summary>
/// Canvas�� ��� UI ��ü�� �������� �����ϱ� ���� ��Ʈ ������Ʈ �̴�.
/// ��� UI ���� ��ҵ��� ��� ĵ���� �ؿ� ��ġ�Ѵ�(ĵ������ �ڽ�)
/// 3������ ������尡 ������ Screen Space - Camera ������� �Ѵ�.
/// �� ���� ĵ������ �׻� ī�޶� ����ٴϸ� ī�޶� �տ� ��ġ�ϰ� �ȴ�.
/// ���� ��ǥ �󿡼��� ��ġ ���� ������ �ȴ�.
/// </summary>
class Canvas : public UI
{
public:
	NewbieEngine_DLL Canvas(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~Canvas();

private:
	shared_ptr<GameObject> _camera;		// UI�� �������� ī�޶� ���

	shared_ptr<Transform> _transform;

public:
	NewbieEngine_DLL void SetCamera(shared_ptr<GameObject> camera);

	void Update() override;
};

