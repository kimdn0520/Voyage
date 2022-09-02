#pragma once
#include "GraphicsEngineAPI.h"
#include "Device.h"

/// <summary>
/// 메시 클래스 입니다.
/// </summary>
class Mesh
{
public:
	Mesh();
	~Mesh();

private:
	ComPtr<ID3D11Buffer> _vertexBuffer;

	ComPtr<ID3D11Buffer> _indexBuffer;

	ComPtr<ID3D11RasterizerState> _rasterState;

	D3D11_PRIMITIVE_TOPOLOGY _topology;			

	unsigned int _indexBufferSize;

public:
	unsigned int stride = 0;

	unsigned int& GetIdxBufferSize() { return _indexBufferSize; }

	const ComPtr<ID3D11Buffer>& GetVertexBuffer() { return _vertexBuffer; }

	const ComPtr<ID3D11Buffer>& GetIndexBuffer() { return _indexBuffer; }

	const ComPtr<ID3D11RasterizerState>& GetRasterState() { return _rasterState; }

	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() { return _topology; }

	void SetRenderState(ComPtr<ID3D11RasterizerState> rasterState) { _rasterState = rasterState; }

	void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { _topology = topology; }

	void SetIndexBufferSize(unsigned int indexBufferSize) { _indexBufferSize = indexBufferSize;  }
	
	template <typename T>
	void CreateVertexBuffer(vector<T> vertices);

	void CreateIndexBuffer(vector<unsigned int> indexBuffer);
};

template<typename T>
inline void Mesh::CreateVertexBuffer(vector<T> vertices)
{
	//---------------------------------------------------------------------------------------------------------
	// 꼭짓점 버퍼를 만드는 과정
	//---------------------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.ByteWidth = sizeof(T) * vertices.size();
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexInitData;
	vertexInitData.pSysMem = vertices.data();

	GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice()->
		CreateBuffer(&vertexBufferDesc, &vertexInitData, _vertexBuffer.GetAddressOf());
}
