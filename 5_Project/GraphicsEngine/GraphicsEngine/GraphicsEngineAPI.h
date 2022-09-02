#pragma once
#include "GraphicsEngineDLL.h"
#include "ConstantBufferDefine.h"

struct GraphicsVertex;
struct MaterialInfo;
struct MeshInfo;
struct TextInfo;
struct CameraInfo;
struct LightInfo;
struct ColDebugInfo;
struct UIInfo;
class RasterizerState;
class Device;
class DepthStencilState;
class BlendState;
class RenderTargetView;
class ParticleInfo;

/// <summary>
/// 뉴비 엔진에서 이녀석을 받아서 쓰고 있기 때문에... 그래픽스엔진.h 쪽에
/// 함수를 만들고싶으면 여기서 virtual abstract 선언 해주고 그 다음
/// override 해서 쓰면 됩니다..
/// </summary>
class GraphicsEngineAPI
{
public:
	GraphicsEngineAPI() {};
	virtual ~GraphicsEngineAPI() {};

private:
	static shared_ptr<GraphicsEngineAPI> graphicsEngine;

public:
	static GraphicsEngine_DLL shared_ptr<GraphicsEngineAPI> GetInstance();

public:
	virtual GraphicsEngine_DLL WindowInfo& GetWindowInfo() abstract;

	virtual GraphicsEngine_DLL shared_ptr<DepthStencilState> GetDepthStencilStateClass() abstract;

	virtual GraphicsEngine_DLL void Init(HWND hwnd, int width, int height) abstract;

	virtual GraphicsEngine_DLL void OnResize(int width, int height) abstract;

	virtual GraphicsEngine_DLL void Release() abstract;

	virtual GraphicsEngine_DLL void BeginRender() abstract;

	virtual GraphicsEngine_DLL void Render() abstract;

	virtual GraphicsEngine_DLL void EndRender() abstract;

	virtual GraphicsEngine_DLL shared_ptr<Device> GetDeviceClass() abstract;

	virtual GraphicsEngine_DLL shared_ptr<RasterizerState> GetWireClass() abstract;

	virtual GraphicsEngine_DLL shared_ptr<RasterizerState> GetSolidClass() abstract;
	
	virtual GraphicsEngine_DLL shared_ptr<RasterizerState> GetSolidNoneCullClass() abstract;

	virtual GraphicsEngine_DLL shared_ptr<BlendState> GetAlphaBlendClass() abstract;

	virtual GraphicsEngine_DLL shared_ptr<RenderTargetView> GetMainRenderTarget() abstract;

	virtual GraphicsEngine_DLL void SetObjInfo(shared_ptr<MeshInfo> mesh) abstract;

	virtual GraphicsEngine_DLL void SetTextInfo(shared_ptr<TextInfo> text) abstract;

	virtual GraphicsEngine_DLL void SetCameraInfo(CameraInfo cameraInfo) abstract;

	virtual GraphicsEngine_DLL void SetLightInfo(LightInfo lightInfo) abstract;

	virtual GraphicsEngine_DLL void SetColDebugInfo(shared_ptr<ColDebugInfo> colDebugInfo) abstract;
	
	virtual GraphicsEngine_DLL void SetUIInfo(shared_ptr<UIInfo> uiInfo) abstract;

	virtual GraphicsEngine_DLL void UpdateLightInfo(LightInfo lightInfo) abstract;

	virtual GraphicsEngine_DLL void SetMesh(vector<GraphicsVertex> vertices, vector<unsigned int> indicies, size_t name, int topology, int rasterState, bool isParticle) abstract;

	virtual GraphicsEngine_DLL void SetBackBufferRenderTarget() abstract;

	virtual GraphicsEngine_DLL void TurnZBufferOn() abstract;

	virtual GraphicsEngine_DLL void TurnZBufferOff() abstract;

	virtual GraphicsEngine_DLL void TurnSkyboxOn() abstract;

	virtual GraphicsEngine_DLL void SetMaterialInfo(shared_ptr<MaterialInfo> materialInfo) abstract;

	virtual GraphicsEngine_DLL void RenewalMaterials() abstract;

	virtual GraphicsEngine_DLL void SetCubeMapTextureInfo(int materialID, wstring cubeMapTextuer) abstract;

	virtual GraphicsEngine_DLL void SetDiffuseMapTextureInfo(int materialID, wstring diffuseMapTexture) abstract;

	virtual GraphicsEngine_DLL void SetNormalMapTextureInfo(int materialID, wstring normalMapTexture) abstract;

	virtual GraphicsEngine_DLL void SetWaterCameraInfo(CameraInfo cameraInfo) abstract;

	virtual GraphicsEngine_DLL void SetIsFade(bool value) abstract;
	
	virtual GraphicsEngine_DLL void SetAlpha(float alpha) abstract;

	virtual GraphicsEngine_DLL void ColDebugSwitch() abstract;

	virtual GraphicsEngine_DLL void SetDeltaTime(float deltatime) abstract;

	virtual GraphicsEngine_DLL void SetParticleDeletIndex(std::vector<int>& deleteindex) abstract;


};
