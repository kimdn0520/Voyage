#pragma once
#include "PassBase.h"

class VertexShader;
class PixelShader;

class ColDebugPass : public PassBase
{
public:
	ColDebugPass();
	~ColDebugPass();

private:
	shared_ptr<VertexShader> _basic_VS;
	shared_ptr<PixelShader> _basic_PS;

	shared_ptr<VertexShader> _light_VS;
	
	shared_ptr<PixelShader> _deferredMulti_PS;

public:
	void Init() override;

	void Render(vector<shared_ptr<ColDebugInfo>> colInfos) override;
};

