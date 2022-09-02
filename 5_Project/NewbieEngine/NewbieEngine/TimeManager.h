#pragma once
#include "NewbieEngineDLL.h"

/// <summary>
/// Update���� 1�����Ӹ��� �ѹ� �� ������.
/// Timer�� ����Ͽ� ���� �����ӿ��� ���� �����ӱ��� ����� �ð�(deltaTime)��
/// ���������ν� 1�ʰ� ������ ��ŭ�� �������� ������ �� �ִ�.
/// </summary>
class TimeManager
{
public:
	TimeManager() {};
	~TimeManager() {};

private:
	static shared_ptr<TimeManager> timeManager;

public:
	static NewbieEngine_DLL shared_ptr<TimeManager> GetInstance();

public:
	void Init();
	void Update();

	unsigned __int32 GetFps() { return _fps; }
	NewbieEngine_DLL float GetDeltaTime() { return _deltaTime; }

private:
	unsigned __int64	_frequency = 0;
	unsigned __int64	_prevCount = 0;
	float	_deltaTime = 0.f;				// ���� �����ӿ��� ���� �����ӱ��� ����� �ð�

private:
	unsigned __int32	_frameCount = 0;	// Update���� ��� ����Ǿ����� �Ի�
	float	_frameTime = 0.f;				// ���� �ð�
	unsigned __int32	_fps = 0;			// framePerSecond �ʸ��� ��������� �������� ����ƴ��� ���
};

