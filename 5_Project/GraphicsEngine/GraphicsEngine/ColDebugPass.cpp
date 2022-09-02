#include "pch.h"
#include "ColDebugPass.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "ConstantBufferDefine.h"

ColDebugPass::ColDebugPass()
{}

ColDebugPass::~ColDebugPass()
{}

void ColDebugPass::Init()
{
	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();

	_basic_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Basic_VS"));
	_basic_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Basic_PS"));

	_light_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS"));	// 야매

	_deferredMulti_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"DeferredMulti_PS"));	// 야매..
}

void ColDebugPass::Render(vector<shared_ptr<ColDebugInfo>> colInfos)
{
	for (auto& colInfo : colInfos)
	{
		switch (colInfo->type)
		{
		case ColliderType::Box:
		{
			cbMesh cbMeshBuffer;
			cbMeshBuffer.gWorld = colInfo->worldTM;
			cbMeshBuffer.gWorldViewProj = colInfo->worldTM * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
			XMVECTOR det = XMMatrixDeterminant(colInfo->worldTM);
			cbMeshBuffer.gWorldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, colInfo->worldTM));
			cbMeshBuffer.objectID = 0;	// 야매..

			_light_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_light_VS->Update();

			_deferredMulti_PS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_deferredMulti_PS->ConstantBufferUpdate(&ResourceManager::GetInstance()->cbMaterialBuffer, "cbMaterial");
			_deferredMulti_PS->Update();

			// 어떻게 그릴지
			g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetWireClass()->GetRasterizerState().Get());

			// Set vertex buffer stride and offset
			unsigned int stride = ResourceManager::GetInstance()->GetMesh((int)MeshName::WireGreenCubeMesh)->stride;
			unsigned int offset = 0;

			if (!colInfo->isCol)
				// 어떤 방식으로 그릴지
			{
				g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh((int)MeshName::WireGreenCubeMesh)->GetPrimitiveTopology());

				// 버텍스 버퍼 설정
				g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh((int)MeshName::WireGreenCubeMesh)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

				// 인덱스 버퍼 설정
				g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh((int)MeshName::WireGreenCubeMesh)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

				// 그린다!
				g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh((int)MeshName::WireGreenCubeMesh)->GetIdxBufferSize(), 0, 0);
			}
			else
			{
				g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh((int)MeshName::WireRedCubeMesh)->GetPrimitiveTopology());

				// 버텍스 버퍼 설정
				g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh((int)MeshName::WireRedCubeMesh)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

				// 인덱스 버퍼 설정
				g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh((int)MeshName::WireRedCubeMesh)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

				// 그린다!
				g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh((int)MeshName::WireRedCubeMesh)->GetIdxBufferSize(), 0, 0);
			}
		}
		break;
		case ColliderType::Sphere:
		{
			cbMesh cbMeshBuffer;
			cbMeshBuffer.gWorld = colInfo->worldTM;
			cbMeshBuffer.gWorldViewProj = colInfo->worldTM * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
			XMVECTOR det = XMMatrixDeterminant(colInfo->worldTM);
			cbMeshBuffer.gWorldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, colInfo->worldTM));
			cbMeshBuffer.objectID = 0;	// 야매..

			_light_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_light_VS->Update();

			_deferredMulti_PS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_deferredMulti_PS->ConstantBufferUpdate(&ResourceManager::GetInstance()->cbMaterialBuffer, "cbMaterial");
			_deferredMulti_PS->Update();

			// 어떻게 그릴지
			g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetWireClass()->GetRasterizerState().Get());

			// Set vertex buffer stride and offset
			unsigned int stride = ResourceManager::GetInstance()->GetMesh((int)MeshName::WireGreenSphereMesh)->stride;
			unsigned int offset = 0;

			if (!colInfo->isCol)
				// 어떤 방식으로 그릴지
			{
				g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh((int)MeshName::WireGreenSphereMesh)->GetPrimitiveTopology());

				// 버텍스 버퍼 설정
				g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh((int)MeshName::WireGreenSphereMesh)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

				// 인덱스 버퍼 설정
				g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh((int)MeshName::WireGreenSphereMesh)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

				// 그린다!
				g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh((int)MeshName::WireGreenSphereMesh)->GetIdxBufferSize(), 0, 0);
			}
			else
			{
				g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh((int)MeshName::WireRedSphereMesh)->GetPrimitiveTopology());

				// 버텍스 버퍼 설정
				g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh((int)MeshName::WireRedSphereMesh)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

				// 인덱스 버퍼 설정
				g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh((int)MeshName::WireRedSphereMesh)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

				// 그린다!
				g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh((int)MeshName::WireRedSphereMesh)->GetIdxBufferSize(), 0, 0);
			}
		}
		break;
		}
	}
}

