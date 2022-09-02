#pragma once
#include "PassBase.h"
#include "ConstantBufferDefine.h"

class VertexShader;
class PixelShader;
class RenderTargetView;
class DebugWindow;
constexpr int DEFERRED_COUNT = 5;

/// <summary>
/// 라이팅 연산을 후 처리 해주는 Pass 그래서 지연 렌더링(디퍼드 렌더링) 이라고 한다.
/// 
/// </summary>
class DeferredPass : public PassBase
{
public:
	DeferredPass();
	~DeferredPass();

private:
	shared_ptr<VertexShader> _basic_VS;
	shared_ptr<PixelShader> _basic_PS;

	shared_ptr<PixelShader> _particle_PS;

	shared_ptr<VertexShader> _deferred_VS;
	shared_ptr<PixelShader> _deferred_PS;
	shared_ptr<PixelShader> _deferredMulti_PS;

	shared_ptr<VertexShader> _light_VS;
	shared_ptr<VertexShader> _light_VS_1;

	shared_ptr<VertexShader> _skyBox_VS;
	shared_ptr<PixelShader> _skyBox_PS;

	// 물을 위한 쉐이더
	shared_ptr<VertexShader> _water_VS;
	shared_ptr<PixelShader> _water_PS;

	//-------------------------------------------------------------------------------------
	//						Deferred를 위해 필요한 것들..!
	//-------------------------------------------------------------------------------------
	shared_ptr<RenderTargetView> _gBuffers[DEFERRED_COUNT];
	ComPtr<ID3D11RenderTargetView> _gBufferViews[DEFERRED_COUNT];
	shared_ptr<DebugWindow>	 _deferredWindow;
	cbTexture _cbTextureBuf;

	ComPtr<ID3D11ShaderResourceView> _gWaterSRV;
	ComPtr<ID3D11ShaderResourceView> _gReflectionSkySRV;
	ComPtr<ID3D11ShaderResourceView> _gReflectionObjectSRV;

	float _waterTranslation = 0.f;

public:
	void Init() override;

	void Release();

	void OnResize(int width, int height);

	void BeginRender(/*OBJECT_TYPE type*/);

	//void RenderDynamicCube(int cubeMapFaceIndex);

	//void RetrurnBeginRender();

	void SetDynamicCubeSRV(ComPtr<ID3D11ShaderResourceView> watersrv) { _gWaterSRV = watersrv; }
	void SetReflectionSkySRV(ComPtr<ID3D11ShaderResourceView> reflectionskysrv) { _gReflectionSkySRV = reflectionskysrv; }
	void SetReflectionObjectSRV(ComPtr<ID3D11ShaderResourceView> reflectionobjectsrv) { _gReflectionObjectSRV = reflectionobjectsrv; }


	void Render(vector<shared_ptr<MeshInfo>> meshs) override;

	void EndRender();
};

