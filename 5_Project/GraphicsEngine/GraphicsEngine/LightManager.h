#pragma once
#include "ConstantBufferDefine.h"

enum class LIGHT_TYPE
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

/// <summary>
/// ���� �������� ����Ʈ ������ �Ѱ��ָ� ���⼭ ����Ʈ���� ���� ���ش�.
/// Pass ������ static���� cbLight ����ü ������ �Ѱ��ְ� �ִ�.
/// </summary>
class LightManager
{
public:
	LightManager() {};
	~LightManager() {};

private:
	static shared_ptr<LightManager> lightManager;

public:
	static shared_ptr<LightManager> GetInstance();

public:
	// �̸�, ������ ����
	static map<size_t, LightInfo> dirLights;
	static map<size_t, LightInfo> pointLights;
	static map<size_t, LightInfo> spotLights;

	// �������� �� light constantBuffer
	static cbLight		cbLightBuffer;

public:
	void SetLightInfo(LightInfo lightInfo);

	void UpdateLightInfo(LightInfo lightInfo);

	void ResetLightInfo();

	void Init();

	void Update();
};


