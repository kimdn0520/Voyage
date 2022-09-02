#pragma once
#include "PassBase.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class DebugWindow;
constexpr int WATER_COUNT = 2;

class WaterPass : public PassBase
{
public:
	WaterPass();
	~WaterPass();

private:

	shared_ptr<VertexShader> _deferred_VS;
	shared_ptr<PixelShader> _waterPassDefault;

	shared_ptr<VertexShader> _light_VS;
	shared_ptr<VertexShader> _light_VS_1;

	shared_ptr<VertexShader> _skyBox_VS;
	shared_ptr<PixelShader> _skyBox_PS;

	shared_ptr<VertexShader> _water_VS;
	shared_ptr<PixelShader> _water_PS;
	
	//-------------------------------------------------------------------------------------
	//						water를 위해 필요한 것들..!
	//-------------------------------------------------------------------------------------
	shared_ptr<RenderTargetView> _gWaterBuffers;
	ComPtr<ID3D11RenderTargetView> _gWaterViews;
	

	// 추후 정보를 옮겨야함
	float _waterTranslation = 0.f;
	float _waterHeight=0.1f;

public:
	void Init() override;

	void Release();

	void OnResize(int width, int height);

	void BeginRender();

	void RenderDynamicCube(int i);

	ComPtr<ID3D11ShaderResourceView> GetWaterSRV();

	void Render(vector<shared_ptr<MeshInfo>> meshs) override;

};


