#pragma once

class IEventState
{
public:
	// �̺�Ʈ�� ���� ������
	virtual void Start() = 0;
	
	// �̺�Ʈ�� ������Ʈ ������
	virtual int Update() = 0;
	
	// �̺�Ʈ�� ������
	virtual void End() = 0;
};