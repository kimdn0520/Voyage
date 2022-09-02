#pragma once
#include "PassBase.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class DebugWindow;

class UIPass : public PassBase
{
public:
	UIPass();
	~UIPass();

private:
	shared_ptr<VertexShader> _UI_VS;
	shared_ptr<PixelShader> _UI_PS;

	shared_ptr<VertexShader> _basic_VS;
	shared_ptr<PixelShader> _basic_PS_1;

	shared_ptr<RenderTargetView> _renderTextureClass;
	shared_ptr<DebugWindow>	 _debugWindow;
	ComPtr<ID3D11RenderTargetView> _renderTargetViewTexture;

public:
	static float alpha;

	static bool isFade;

public:
	void Init() override;

	void Release();

	void BeginRender();

	void Render(vector<shared_ptr<UIInfo>> uiInfos) override;

	void EndRender();
};

