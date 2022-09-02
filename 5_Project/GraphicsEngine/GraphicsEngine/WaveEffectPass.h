#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;

class WaveEffectPass : public PassBase
{
public:
	WaveEffectPass();
	~WaveEffectPass();

private:
	shared_ptr<VertexShader> _waveEffect_VS;
	shared_ptr<PixelShader> _waveEffect_PS;

	float _deltaTime=0.f;
public:
	void Init() override;

	void BeginRender();

	void Render(vector<shared_ptr<MeshInfo>> meshs) override;

	void EndRender();
};

