#pragma once

class GraphicsEngineAPI;

class PassBase
{
public:
	PassBase();
	~PassBase();

private:
protected:
	ComPtr<ID3D11DeviceContext> g_deviceContext;
	ComPtr<ID3D11Device> g_device;
	shared_ptr<GraphicsEngineAPI> g_graphics;
	
public:
	virtual void Init() abstract;

	virtual void Render(vector<shared_ptr<MeshInfo>> meshs) {};

	virtual void Render(vector<shared_ptr<ColDebugInfo>> colInfos) {};

	virtual void Render(vector<shared_ptr<UIInfo>> uiInfos) {};
};

