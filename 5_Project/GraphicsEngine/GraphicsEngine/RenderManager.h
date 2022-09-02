#pragma once

class BasicPass;
class LightPass;
class DeferredPass;
class DebugPass;
class TextPass;
class ColDebugPass;
class UIPass;
class WaterPass;
class OceanTestPass;
class ReflectionSkyPass;
class ReflectionObjectPass;
class PreBloomPass;
class BloomPass;
class WaveEffectPass;

/// <summary>
/// �׸����� ���θ� ����ϴ� Ŭ���� �Դϴ�.
/// ���� pass �� ��� �ֽ��ϴ�. _renderData, _textData... �� ���⿡ �׸� �޽õ��� �־�ΰ�
/// pass->Render ���� �� �޽õ��� Draw ���ְ� �ִ�.
/// </summary>
class RenderManager
{
public:
	RenderManager() {};
	~RenderManager() {};

private:
	static shared_ptr<RenderManager> renderManager;

public:
	static shared_ptr<RenderManager> GetInstance();

	static CameraInfo s_cameraInfo;

	static std::vector<CameraInfo> s_waterCameraInfo;

private:
	shared_ptr<DebugPass> _debugPass;
	shared_ptr<TextPass> _textPass;
	shared_ptr<DeferredPass> _deferredPass;
	shared_ptr<ColDebugPass> _colDebugPass;
	shared_ptr<UIPass> _UIPass;
	shared_ptr< WaterPass> _WaterPass;
	shared_ptr<ReflectionSkyPass> _ReflectionSkyPass;
	shared_ptr<ReflectionObjectPass> _ReflectionObjectPass;
	shared_ptr<PreBloomPass> _PreBloomPass;
	shared_ptr<BloomPass> _BloomPass;
	shared_ptr<WaveEffectPass> _WavePass;


	// test
	shared_ptr<OceanTestPass> _OceanPass;

	vector<shared_ptr<MeshInfo>> _renderData;

	vector<shared_ptr<TextInfo>> _textData;

	vector<shared_ptr<UIInfo>> _uiData;		

	vector<shared_ptr<ColDebugInfo>> _colDebugData;


public:
	bool isInit = false;

	bool isColDebugSwitch = false;

public:
	void Init();

	void OnResize(int width, int height);

	void Release();

	void PushRenderData(shared_ptr<MeshInfo> meshInfo);

	void PushTextData(shared_ptr<TextInfo> textInfo) { _textData.push_back(textInfo); }

	void PushColDebugData(shared_ptr<ColDebugInfo> colDebugInfo) { _colDebugData.push_back(colDebugInfo); }

	void PushUIData(shared_ptr<UIInfo> uiInfo) { _uiData.push_back(uiInfo); }

	void Render();

	void TextRender();

	void DebugRender();		// MRT �����

	void DeferredRender();

	void ColDebugRender();

	void UIRender();

	void WaterRender();

	void ReflectionRender();

	void PostProcessing();

};

