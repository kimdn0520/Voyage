#include "pch.h"
#include "Mesh.h"

Mesh::Mesh()
{}

Mesh::~Mesh()
{}

void Mesh::CreateIndexBuffer(vector<unsigned int> indices)
{
	//---------------------------------------------------------------------------------------------------------
	// �ε��� ���۸� ����� ����
	//---------------------------------------------------------------------------------------------------------
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indices.size();
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInitData;
	indexInitData.pSysMem = indices.data();

	_indexBufferSize = indices.size();

	GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice()->
		CreateBuffer(&indexBufferDesc, &indexInitData, _indexBuffer.GetAddressOf());
}
