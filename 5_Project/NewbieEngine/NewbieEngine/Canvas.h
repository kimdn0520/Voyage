#pragma once
#include "NewbieEngineDLL.h"
#include "UI.h"

class Transform;

/// <summary>
/// Canvas는 모든 UI 객체의 렌더링을 관리하기 위한 루트 컴포넌트 이다.
/// 모든 UI 구성 요소들은 모두 캔버스 밑에 위치한다(캔버스의 자식)
/// 3가지의 렌더모드가 있지만 Screen Space - Camera 방식으로 한다.
/// 이 모드는 캔버스는 항상 카메라를 따라다니며 카메라 앞에 위치하게 된다.
/// 따라서 좌표 상에서의 위치 값을 가지게 된다.
/// </summary>
class Canvas : public UI
{
public:
	NewbieEngine_DLL Canvas(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~Canvas();

private:
	shared_ptr<GameObject> _camera;		// UI를 렌더해줄 카메라 등록

	shared_ptr<Transform> _transform;

public:
	NewbieEngine_DLL void SetCamera(shared_ptr<GameObject> camera);

	void Update() override;
};

