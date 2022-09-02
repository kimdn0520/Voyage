#pragma once
#include "PassBase.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class DebugWindow;

class DebugPass : public PassBase
{
public:
	DebugPass();
	~DebugPass();

private:
	shared_ptr<VertexShader> _basic_VS;
	shared_ptr<PixelShader> _basic_PS_1;
	shared_ptr<VertexShader> _light_VS;
	shared_ptr<VertexShader> _light_VS_1;
	shared_ptr<PixelShader> _debug_PS;

	shared_ptr<VertexShader> _skyBox_VS;
	shared_ptr<PixelShader> _skyBox_PS;

	//-------------------------------------------------------------------------------------
	//						Multi Render Target 을 위해 필요한 것들..!
	//-------------------------------------------------------------------------------------
	shared_ptr<RenderTargetView> _renderTextureClass[4];
	shared_ptr<DebugWindow>	 _debugWindow[4];
	ComPtr<ID3D11RenderTargetView> _renderTargetViewTexture[4];

public:
	void Init() override;

	void Release();

	void BeginRender();

	void Render(vector<shared_ptr<MeshInfo>> meshs) override;
	
	void EndRender();
};

