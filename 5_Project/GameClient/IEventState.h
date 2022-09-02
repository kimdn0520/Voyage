#pragma once

class IEventState
{
public:
	// 이벤트가 시작 됐을때
	virtual void Start() = 0;
	
	// 이벤트가 업데이트 됐을때
	virtual int Update() = 0;
	
	// 이벤트가 끝날때
	virtual void End() = 0;
};