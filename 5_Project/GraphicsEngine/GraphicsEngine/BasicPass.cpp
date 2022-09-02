#include "pch.h"
//#include "BasicPass.h"
//#include "ShaderManager.h"
//#include "ShaderBase.h"
//#include "VertexShader.h"
//#include "PixelShader.h"
//#include "ConstantBufferDefine.h"
//#include "ResourceManager.h"
//#include "RenderManager.h"
//#include "Mesh.h"
//
//BasicPass::BasicPass()
//{}
//
//BasicPass::~BasicPass()
//{}
//
//void BasicPass::Init()
//{
//	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();
//
//	_basic_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Basic_VS"));
//	_basic_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Basic_PS"));
//	_basic_PS_1 = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Basic_PS_1"));	// 텍스쳐가 있다면 이걸쓴다.
//}
//
//void BasicPass::Render(vector<shared_ptr<MeshInfo>> meshs)
//{
//	for (auto& mesh : meshs)
//	{
//		for (int matCnt = 0; matCnt < mesh->materials.size(); matCnt++)
//		{
//			cbMesh cbMeshBuffer;
//			cbMeshBuffer.gWorld = mesh->worldTM;
//			cbMeshBuffer.gWorldViewProj = mesh->worldTM * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
//
//			_basic_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
//			_basic_VS->Update();
//
//			if (mesh->materials[matCnt]->isDiffuse)
//			{
//				_basic_PS_1->SetResourceViewBuffer(mesh->materials[matCnt]->diffuseTexture, "gDiffuseMap");
//				_basic_PS_1->Update();
//			}
//			else
//			{
//				_basic_PS->Update();
//			}
//
//			// Set vertex buffer stride and offset
//			unsigned int stride = ResourceManager::GetInstance()->GetMesh(mesh->name)->stride;
//			unsigned int offset = 0;
//
//			// 어떤 방식으로 그릴지
//			g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetPrimitiveTopology());
//
//			// 버텍스버퍼 설정
//			g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh(mesh->name)->GetVertexBuffer().GetAddressOf(), &stride, &offset);
//
//			// 인덱스 버퍼 설정
//			g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//			// 어떻게 그릴지
//			g_deviceContext->RSSetState(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetRasterState().Get());
//
//			// 그린다!
//			g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIdxBufferSize(), 0, 0);
//		}
//	}
//}
