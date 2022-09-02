#include "pch.h"
//#include "LightPass.h"
//#include "ShaderManager.h"
//#include "VertexShader.h"
//#include "PixelShader.h"
//#include "ConstantBufferDefine.h"
//#include "ResourceManager.h"
//#include "Mesh.h"
//#include "ShaderBase.h"
//#include "LightManager.h"
//#include "RenderManager.h"
//
//LightPass::LightPass()
//{}
//
//LightPass::~LightPass()
//{}
//
//void LightPass::Init()
//{
//	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();
//
//	_basic_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Basic_VS"));
//	_basic_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Basic_PS"));
//	
//	_light_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS"));		 // 디폴트
//	_light_VS_1 = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"Light_VS_1"));	 // 스킨드 적용 
//	_light_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"Light_PS"));	 
//
//	_skyBox_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_VS"));
//	_skyBox_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"SkyBox_PS"));
//}
//
//void LightPass::Render(vector<shared_ptr<MeshInfo>> meshs)
//{	
//	for (auto& mesh : meshs)
//	{
//		for (int matCnt = 0; matCnt < mesh->materials.size(); matCnt++)
//		{
//			if (mesh->type == OBJECT_TYPE::SKY_BOX)
//				continue;
//
//			switch (mesh->type)
//			{
//				case OBJECT_TYPE::DEFAULT:
//				{
//					cbMesh cbMeshBuffer;
//					cbMeshBuffer.gWorld = mesh->worldTM;
//					cbMeshBuffer.gWorldViewProj = mesh->worldTM * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
//					XMVECTOR det = XMMatrixDeterminant(mesh->worldTM);
//					cbMeshBuffer.gWorldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, mesh->worldTM));
//
//					if (mesh->isSkinned)
//					{
//						cbSkinned cbSkinnedBuffer;
//						memcpy(&cbSkinnedBuffer.gBoneTransforms, mesh->finalBoneListMatrix, sizeof(Matrix) * 96);
//						_light_VS_1->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
//						_light_VS_1->ConstantBufferUpdate(&cbSkinnedBuffer, "cbSkinned");
//						_light_VS_1->Update();
//					}
//					else
//					{
//						_light_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
//						_light_VS->Update();
//					}
//
//					cbMaterial cbMaterialBuffer;
//					cbMaterialBuffer.gMaterialAmbient = mesh->materials[matCnt]->ambient;
//					cbMaterialBuffer.gMaterialDiffuse = mesh->materials[matCnt]->diffuse;
//					cbMaterialBuffer.gMaterialSpecular = mesh->materials[matCnt]->specular;
//					cbMaterialBuffer.isDiffuseTexture = mesh->materials[matCnt]->isDiffuse;
//					cbMaterialBuffer.isNormalTexture = mesh->materials[matCnt]->isNormal;
//					cbMaterialBuffer.isSpecularTexture = mesh->materials[matCnt]->isSpecular;
//
//					if (mesh->materials[matCnt]->isSpecular)
//					{
//						_light_PS->SetResourceViewBuffer(mesh->materials[matCnt]->specularTexture, "gSpecularMap");
//					}
//
//					if (mesh->materials[matCnt]->isNormal)
//					{
//						_light_PS->SetResourceViewBuffer(mesh->materials[matCnt]->normalTexture, "gNormalMap");
//					}
//
//					if (mesh->materials[matCnt]->isDiffuse)
//					{
//						_light_PS->SetResourceViewBuffer(mesh->materials[matCnt]->diffuseTexture, "gDiffuseMap");
//					}
//
//					_light_PS->ConstantBufferUpdate(&cbMaterialBuffer, "cbMaterial");
//					_light_PS->ConstantBufferUpdate(&LightManager::cbLightBuffer, "cbLight");
//					_light_PS->Update();
//
//					// 어떻게 그릴지
//					g_deviceContext->RSSetState(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetRasterState().Get());
//				}
//				break;
//				case OBJECT_TYPE::UTIL:
//				{
//					cbMesh cbMeshBuffer;
//					cbMeshBuffer.gWorld = mesh->worldTM;
//					cbMeshBuffer.gWorldViewProj = mesh->worldTM * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
//
//					_basic_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
//					_basic_VS->Update();
//
//					_basic_PS->Update();
//
//					// 어떻게 그릴지
//					g_deviceContext->RSSetState(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetRasterState().Get());
//				}
//				break;
//				case OBJECT_TYPE::SKY_BOX:
//				{
//					cbMesh cbMeshBuffer;
//					Matrix camPos = XMMatrixTranslationFromVector(RenderManager::s_cameraInfo.worldPos);
//					cbMeshBuffer.gWorldViewProj = camPos * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
//				
//					_skyBox_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
//					_skyBox_VS->Update();
//
//					_skyBox_PS->SetResourceViewBuffer(mesh->materials[matCnt]->cubeMapTexture, "gCubeMap");
//					_skyBox_PS->Update();
//
//					// 어떻게 그릴지
//					g_deviceContext->RSSetState(GraphicsEngineAPI::GetInstance()->GetSolidNoneCullClass()->GetRasterizerState().Get());
//				}
//				break;
//			}
//
//			// Set vertex buffer stride and offset
//			unsigned int stride = ResourceManager::GetInstance()->GetMesh(mesh->name)->stride;
//			unsigned int offset = 0;
//
//			// 어떤 방식으로 그릴지
//			g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetPrimitiveTopology());
//
//			// 버텍스 버퍼 설정
//			g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh(mesh->name)->GetVertexBuffer().GetAddressOf(), &stride, &offset);
//
//			// 인덱스 버퍼 설정
//			g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//			// 그린다!
//			if (!mesh->isBone)
//				g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIdxBufferSize(), 0, 0);
//		}
//	}
//}
