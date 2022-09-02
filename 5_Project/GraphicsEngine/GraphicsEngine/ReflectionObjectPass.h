#pragma once
#include "PassBase.h"

class VertexShader;
class PixelShader;
class RenderTargetView;

class ReflectionObjectPass : public PassBase
{
public:
	ReflectionObjectPass();
	~ReflectionObjectPass();

private:
	shared_ptr<VertexShader> _deferred_VS;
	shared_ptr<PixelShader> _waterPassDefault;


	shared_ptr<VertexShader> _light_VS;
	shared_ptr<VertexShader> _light_VS_1;

	shared_ptr<VertexShader> _skyBox_VS;
	shared_ptr<PixelShader> _skyBox_PS;

	// ���� ���� ���̴�
	shared_ptr<VertexShader> _water_VS;
	shared_ptr<PixelShader> _water_PS;

	// Object reflection�� ����� ���� ���� Ÿ��
	shared_ptr<RenderTargetView> _gObjectBuffer;
	ComPtr<ID3D11RenderTargetView> _gObjectBufferView;

public:

	void Init() override;

	void Release();

	void BeginRender();

	void Render(vector<shared_ptr<MeshInfo>> meshs) override;

	ComPtr<ID3D11ShaderResourceView> GetObjectSRV();
};

