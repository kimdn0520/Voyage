#pragma once
#include "ConstantBufferDefine.h"

enum class LIGHT_TYPE
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

/// <summary>
/// 뉴비 엔진에서 라이트 정보를 넘겨주면 여기서 라이트들을 관리 해준다.
/// Pass 쪽으로 static으루 cbLight 구조체 정보를 넘겨주고 있다.
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
	// 이름, 정보가 들어간댜
	static map<size_t, LightInfo> dirLights;
	static map<size_t, LightInfo> pointLights;
	static map<size_t, LightInfo> spotLights;

	// 전역으로 쓸 light constantBuffer
	static cbLight		cbLightBuffer;

public:
	void SetLightInfo(LightInfo lightInfo);

	void UpdateLightInfo(LightInfo lightInfo);

	void ResetLightInfo();

	void Init();

	void Update();
};


