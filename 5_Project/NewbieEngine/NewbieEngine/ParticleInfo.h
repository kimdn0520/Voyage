#pragma once
#include "NewbieEngineDLL.h"
#include "ObjectInfo.h"

/// <summary>
/// 처음 파티클의 정보를 저장하는 함수
/// </summary>
class ParticleInfo
{
public:
	NewbieEngine_DLL ParticleInfo() {};
	NewbieEngine_DLL ~ParticleInfo() {};

public:
	unsigned int materialID;

	// 방출 될 때 파티클이 위치 할 수 있는 임의의 편차를 설정합니다.
	float _particleDeviationX = 0;
	float _particleDeviationY = 0;
	float _particleDeviationZ = 0;

	// 파티클의 속도와 속도 변화를 설정합니다.
	float _particleVelocity = 0;
	float _particleVelocityVariation = 0;

	// 파티클의 물리적 크기를 설정합니다.
	float _particleSize = 0;

	// 초당 방출 할 파티클 수를 설정합니다.
	float _particlesPerSecond = 0;

	// 파티클 시스템에 허용되는 최대 파티클 수를 설정합니다.
	int _maxParticles = 0;

	// 아직 배출되지 않으므로 현재 파티클 수를 0으로 초기화합니다.
	int _currentParticleCount = 0;

	// 초당 파티클 방출 속도의 초기 누적 시간을 지웁니다.
	float _accumulatedTime = 0;

	std::vector<std::shared_ptr<ParticleType>> _particleList;


};

