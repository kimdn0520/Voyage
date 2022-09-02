#pragma once
#include "GraphicsEngineAPI.h"

class Device;
class SwapChain;
class RenderTargetView;
class DepthStencilState;
class RasterizerState;
class DebugWindow;
class BlendState;

class GraphicsEngine : public GraphicsEngineAPI
{
private:
	shared_ptr<Device> _deviceClass;
	shared_ptr<SwapChain> _swapChainClass;
	shared_ptr<RenderTargetView> _mainRenderTargetViewClass;

	shared_ptr<DepthStencilState> _depthStencilStateClass;
	shared_ptr<DepthStencilState> _disableDepthStencilStateClass;

	shared_ptr<DepthStencilState> _depthStencilStateClassSkybox;

	shared_ptr<RasterizerState> _solidRasterizerStateClass;

	shared_ptr<RasterizerState> _wireRasterizerStateClass;

	shared_ptr<RasterizerState> _solidNoneCullRasterizerStateClass;

	shared_ptr<BlendState> _alphaBlendStateClass;

private:
	ComPtr<ID3D11Debug> _debug;

	bool _vsync_enabled = false;

public:
	static WindowInfo windowInfo;
	static bool _enable4xMsaa;

public:
	WindowInfo& GetWindowInfo() { return windowInfo; }

	shared_ptr<Device> GetDeviceClass() { return _deviceClass; }

	shared_ptr<RenderTargetView> GetMainRenderTarget() override { return _mainRenderTargetViewClass; }

	shared_ptr<DepthStencilState> GetDepthStencilStateClass() override { return _depthStencilStateClass; }

	shared_ptr<RasterizerState> GetWireClass() override { return _wireRasterizerStateClass; }

	shared_ptr<RasterizerState> GetSolidClass() override { return _solidRasterizerStateClass; }

	shared_ptr<RasterizerState> GetSolidNoneCullClass() override { return _solidNoneCullRasterizerStateClass; }

	shared_ptr<BlendState> GetAlphaBlendClass() override { return _alphaBlendStateClass; }

	// depthStencilState의 Z버퍼(DepthBuffer)를 켜준다.
	void TurnZBufferOn() override;
	
	// depthStencilState의 Z버퍼(DepthBuffer)를 꺼준다.
	// 2D화면을 그리기 위해서는 Z버퍼를 사용하지 않아야 픽셀에 새로운 색상을 덮을 수 있다.
	void TurnZBufferOff() override;

	void TurnSkyboxOn() override;
	
	void Init(HWND hwnd, int width, int height) override;

	void OnResize(int width, int height) override;

	void Release() override;

	// 메인렌더타겟뷰를 초기화 해주는 함수임
	void BeginRender() override;

	// 라이트 정보가 LightManager에서 계속 돌아가고 그 정보를 가지고
	// RenderManager에서 렌더벡터에 있는 놈들과 연산을 하면서 그려준다..
	void Render() override;

	// 화면에 뿅!
	void EndRender() override;

	// 뉴비엔진에서 이 함수로 렌더매니저의 렌더벡터(렌더할 메시들)에 푸시하는 함수임
	void SetObjInfo(shared_ptr<MeshInfo> mesh) override;

	// 뉴비엔진에서 이 함수로 렌더매니저의 텍스트렌더벡터(렌더할 텍스트)에 푸시하는 함수임
	void SetTextInfo(shared_ptr<TextInfo> text) override;

	// 뉴비엔진에서 이 함수로 메인카메라의 worldPos, viewTM, projTM 을 그래픽스엔진의 RenderManager에 보내주는 함수임
	void SetCameraInfo(CameraInfo cameraInfo) override;

	// 뉴비엔진에서 이 함수로 그래픽스엔진의 LightManager에 씬에 존재하는 빛오브젝트 정보를 넘겨주는 함수임
	void SetLightInfo(LightInfo lightInfo) override;

	// 뉴비엔진에서 이 함수로 렌더매니저의 디버그콜라이더벡터에 푸시하는 함수임
	void SetColDebugInfo(shared_ptr<ColDebugInfo> colDebugInfo) override;

	// 뉴비엔진에서 이 함수로 렌더매니저의 UI벡터에 푸시하는 함수임
	void SetUIInfo(shared_ptr<UIInfo> uiInfo) override;

	// 뉴비엔진에서 이 함수로 그래픽스엔진의 LightManager에 씬에 존재하는 빛 오브젝트 정보를 지속적으로 업데이트 시켜주는 함수임
	// 위치라던가.. 바뀌어야 하는 정보들?
	void UpdateLightInfo(LightInfo lightInfo) override;

	// 뉴비엔진에서 mesh 정보들을 그래픽스 엔진의 ResourceManager로 넘겨서 메시를 실질적으로 만들어주는 함수임
	void SetMesh(vector<GraphicsVertex> vertices, vector<unsigned int> indicies, size_t name, int topology, int rasterState, bool isParticle) override;

	// 렌더타겟을 메인렌더타겟으로 다시 바꿔주는 함수임
	void SetBackBufferRenderTarget() override;

	// 뉴비엔진안에서 Material을 받는 함수임
	void SetMaterialInfo(shared_ptr<MaterialInfo> materialInfo) override;

	// 뉴비엔진안에서 그래픽스의 Material 정보를 리뉴얼 시켜주는 함수임
	void RenewalMaterials() override;

	// 뉴비엔진에서 받아온 cubemaptexture를 넣어준다. 
	void SetCubeMapTextureInfo(int materialID, wstring cubeMapTexture) override;

	// 뉴비엔진에서 받아온 diffusetexture를 넣어준다.
	void SetDiffuseMapTextureInfo(int materialID, wstring diffuseMapTexture) override;

	// 뉴비엔진에서 받아온 normaltexture를 넣어준다.
	void SetNormalMapTextureInfo(int materialID, wstring normalMapTexture) override;

	// 뉴비엔진에서 받아온 CameraInfo를 넣어준다
	void SetWaterCameraInfo(CameraInfo cameraInfo) override;

	void SetIsFade(bool value) override;

	void SetAlpha(float alpha) override;

	void ColDebugSwitch() override;

	void SetDeltaTime(float deltatime) override;

	void SetParticleDeletIndex(std::vector<int>& deleteindex) override;
};

