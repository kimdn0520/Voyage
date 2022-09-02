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

	// depthStencilState�� Z����(DepthBuffer)�� ���ش�.
	void TurnZBufferOn() override;
	
	// depthStencilState�� Z����(DepthBuffer)�� ���ش�.
	// 2Dȭ���� �׸��� ���ؼ��� Z���۸� ������� �ʾƾ� �ȼ��� ���ο� ������ ���� �� �ִ�.
	void TurnZBufferOff() override;

	void TurnSkyboxOn() override;
	
	void Init(HWND hwnd, int width, int height) override;

	void OnResize(int width, int height) override;

	void Release() override;

	// ���η���Ÿ�ٺ並 �ʱ�ȭ ���ִ� �Լ���
	void BeginRender() override;

	// ����Ʈ ������ LightManager���� ��� ���ư��� �� ������ ������
	// RenderManager���� �������Ϳ� �ִ� ���� ������ �ϸ鼭 �׷��ش�..
	void Render() override;

	// ȭ�鿡 ��!
	void EndRender() override;

	// ���������� �� �Լ��� �����Ŵ����� ��������(������ �޽õ�)�� Ǫ���ϴ� �Լ���
	void SetObjInfo(shared_ptr<MeshInfo> mesh) override;

	// ���������� �� �Լ��� �����Ŵ����� �ؽ�Ʈ��������(������ �ؽ�Ʈ)�� Ǫ���ϴ� �Լ���
	void SetTextInfo(shared_ptr<TextInfo> text) override;

	// ���������� �� �Լ��� ����ī�޶��� worldPos, viewTM, projTM �� �׷��Ƚ������� RenderManager�� �����ִ� �Լ���
	void SetCameraInfo(CameraInfo cameraInfo) override;

	// ���������� �� �Լ��� �׷��Ƚ������� LightManager�� ���� �����ϴ� ��������Ʈ ������ �Ѱ��ִ� �Լ���
	void SetLightInfo(LightInfo lightInfo) override;

	// ���������� �� �Լ��� �����Ŵ����� ������ݶ��̴����Ϳ� Ǫ���ϴ� �Լ���
	void SetColDebugInfo(shared_ptr<ColDebugInfo> colDebugInfo) override;

	// ���������� �� �Լ��� �����Ŵ����� UI���Ϳ� Ǫ���ϴ� �Լ���
	void SetUIInfo(shared_ptr<UIInfo> uiInfo) override;

	// ���������� �� �Լ��� �׷��Ƚ������� LightManager�� ���� �����ϴ� �� ������Ʈ ������ ���������� ������Ʈ �����ִ� �Լ���
	// ��ġ�����.. �ٲ��� �ϴ� ������?
	void UpdateLightInfo(LightInfo lightInfo) override;

	// ���������� mesh �������� �׷��Ƚ� ������ ResourceManager�� �Ѱܼ� �޽ø� ���������� ������ִ� �Լ���
	void SetMesh(vector<GraphicsVertex> vertices, vector<unsigned int> indicies, size_t name, int topology, int rasterState, bool isParticle) override;

	// ����Ÿ���� ���η���Ÿ������ �ٽ� �ٲ��ִ� �Լ���
	void SetBackBufferRenderTarget() override;

	// �������ȿ��� Material�� �޴� �Լ���
	void SetMaterialInfo(shared_ptr<MaterialInfo> materialInfo) override;

	// �������ȿ��� �׷��Ƚ��� Material ������ ������ �����ִ� �Լ���
	void RenewalMaterials() override;

	// ���������� �޾ƿ� cubemaptexture�� �־��ش�. 
	void SetCubeMapTextureInfo(int materialID, wstring cubeMapTexture) override;

	// ���������� �޾ƿ� diffusetexture�� �־��ش�.
	void SetDiffuseMapTextureInfo(int materialID, wstring diffuseMapTexture) override;

	// ���������� �޾ƿ� normaltexture�� �־��ش�.
	void SetNormalMapTextureInfo(int materialID, wstring normalMapTexture) override;

	// ���������� �޾ƿ� CameraInfo�� �־��ش�
	void SetWaterCameraInfo(CameraInfo cameraInfo) override;

	void SetIsFade(bool value) override;

	void SetAlpha(float alpha) override;

	void ColDebugSwitch() override;

	void SetDeltaTime(float deltatime) override;

	void SetParticleDeletIndex(std::vector<int>& deleteindex) override;
};

