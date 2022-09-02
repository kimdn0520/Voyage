#pragma once
#include "NewbieEngineDLL.h"
#include "ObjectInfo.h"

/// <summary>
/// ó�� ��ƼŬ�� ������ �����ϴ� �Լ�
/// </summary>
class ParticleInfo
{
public:
	NewbieEngine_DLL ParticleInfo() {};
	NewbieEngine_DLL ~ParticleInfo() {};

public:
	unsigned int materialID;

	// ���� �� �� ��ƼŬ�� ��ġ �� �� �ִ� ������ ������ �����մϴ�.
	float _particleDeviationX = 0;
	float _particleDeviationY = 0;
	float _particleDeviationZ = 0;

	// ��ƼŬ�� �ӵ��� �ӵ� ��ȭ�� �����մϴ�.
	float _particleVelocity = 0;
	float _particleVelocityVariation = 0;

	// ��ƼŬ�� ������ ũ�⸦ �����մϴ�.
	float _particleSize = 0;

	// �ʴ� ���� �� ��ƼŬ ���� �����մϴ�.
	float _particlesPerSecond = 0;

	// ��ƼŬ �ý��ۿ� ���Ǵ� �ִ� ��ƼŬ ���� �����մϴ�.
	int _maxParticles = 0;

	// ���� ������� �����Ƿ� ���� ��ƼŬ ���� 0���� �ʱ�ȭ�մϴ�.
	int _currentParticleCount = 0;

	// �ʴ� ��ƼŬ ���� �ӵ��� �ʱ� ���� �ð��� ����ϴ�.
	float _accumulatedTime = 0;

	std::vector<std::shared_ptr<ParticleType>> _particleList;


};

