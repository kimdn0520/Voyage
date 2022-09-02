#pragma once

/// <summary>
/// ※ 타이틀로, 새 게임 시작시 값 초기화 해줄것
/// </summary>
class GameManager 
{
public:
	GameManager();
	virtual ~GameManager();

private:
	static shared_ptr<GameManager> gameManager;

public:
	static shared_ptr<GameManager> GetInstance();

	unordered_map<ItemType, int> items;

public:
	Vector3 _moveToPoint;			// 아이템이 그 방향 벡터로 점점 이동하는 벡터
	
	bool isPlayerStop = false;

	bool isRow = false;				// Youth_row 이벤트 용

	float speed = 20.f;

	float scriptSpeed = 1.f;

public:
	void Init();

	void SetObjectMove(Vector3 moveToPoint) { _moveToPoint = moveToPoint; }

	void ClearData();

	void Update();
};

