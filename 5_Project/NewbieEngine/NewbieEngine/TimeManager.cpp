#include "pch.h"
#include "TimeManager.h"
#include "GraphicsEngineManager.h"

shared_ptr<TimeManager> TimeManager::timeManager = nullptr;

shared_ptr<TimeManager> TimeManager::GetInstance()
{
	if (timeManager == nullptr)
		timeManager = make_shared<TimeManager>();

	return timeManager;
}

void TimeManager::Init()
{
	// ���� Ÿ�̸��� ���ļ�(�ʴ� ����)�� ���� ��
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));

	// ���� Ÿ�̸ӷκ��� ���� ������ ���� �ð��� ���� ��
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU Ŭ��
}

/// <summary>
/// �������� ���������� QueryPerformanceCounter �Լ��� ȣ���ϸ� 
/// ����ð����� �߻��� �������� ���� �� ����
/// �׷��� �� ���� �ð� ������ �ƴϱ� ������
/// QueryPerformanceFrequency�Լ��� ��� �ֱ�� ������ �ð� ������ ��ȯ�ؾ� ��.
/// </summary>
void TimeManager::Update()
{
	unsigned __int64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	// ���� Count(currentCount)���� �� Count(preCount)�� �� ����
	// frequency�� �������ָ� ����� �ð�(��)�� ���´�.
	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;

	_frameCount++;
	_frameTime += _deltaTime;

	if (_frameTime > 1.f)	// 1�ʸ� �Ѿ�� ����
	{
		_fps = static_cast<unsigned __int32>(_frameCount / _frameTime);

		_frameTime = 0.f;
		_frameCount = 0;
	}

	//GraphicsEngineManager::GetInstance()->SetDeltaTime(_deltaTime);
	// ������ߴµ� �ʿ������. �ٵ� Ȥ�� ���� ���ܵ�
}

