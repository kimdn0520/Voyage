#pragma once

/// <summary>
/// �� Ÿ��Ʋ��, �� ���� ���۽� �� �ʱ�ȭ ���ٰ�
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
	Vector3 _moveToPoint;			// �������� �� ���� ���ͷ� ���� �̵��ϴ� ����
	
	bool isPlayerStop = false;

	bool isRow = false;				// Youth_row �̺�Ʈ ��

	float speed = 20.f;

	float scriptSpeed = 1.f;

public:
	void Init();

	void SetObjectMove(Vector3 moveToPoint) { _moveToPoint = moveToPoint; }

	void ClearData();

	void Update();
};

