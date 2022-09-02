#pragma once

/// <summary>
/// 2D ����׿� �������� �������� �Ϲ����� ��Ʈ�� �̹����� �ƴ� RTT �ؽ��Ŀ� �׸��°��̴�.
/// ����ó���� ȿ���� ����� �Ҷ� �̰ɷ� ȭ�鿡 ���� �ȴ�.
/// </summary>
class DebugWindow
{
public:
	DebugWindow();
	~DebugWindow();

	bool Init(ComPtr<ID3D11Device> device, int screenWidth, int screenHeight,
		int textureWidth, int textureHeight);

	void Release();

	bool Render(ComPtr<ID3D11DeviceContext> deviceContext, Vector4 pos);

	int GetIndexCount() { return _indexCount; }

private:
	bool InitializeBuffers(ComPtr<ID3D11Device> device);

	bool UpdateBuffers(ComPtr<ID3D11DeviceContext> deviceContext, Vector4 pos);

	void RenderBuffers(ComPtr<ID3D11DeviceContext> deviceContext);

private:
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;

	int _vertexCount;
	int _indexCount;
	int _screenWidth;
	int _screenHeight;

	int _textureWidth;
	int _textureHeight;

	Vector4 _previousPos;
};

