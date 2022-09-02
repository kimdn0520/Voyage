#include "pch.h"
#include "OceanTestPass.h"
#include "ResourceManager.h"
#include "RenderTargetView.h"
#include "DebugWindow.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "LightManager.h"
#include "ReflectionSkyPass.h"

OceanTestPass::OceanTestPass()
{

}

OceanTestPass::~OceanTestPass()
{

}

void OceanTestPass::Init()
{

	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();

	_OceanTest_VS = dynamic_pointer_cast<VertexShader>(ShaderManager::GetInstance()->GetShader(L"OceanTest_VS"));
	_OceanTest_PS = dynamic_pointer_cast<PixelShader>(ShaderManager::GetInstance()->GetShader(L"OceanTest_PS"));

}

void OceanTestPass::Release()
{

}

void OceanTestPass::BeginRender()
{

}

void OceanTestPass::Render(vector<shared_ptr<MeshInfo>> meshs)
{
	for (auto& mesh : meshs)
	{
		switch (mesh->type)
		{
		case OBJECT_TYPE::WATER:
		{
			cbMesh cbMeshBuffer;
			cbMeshBuffer.gWorld = mesh->worldTM;
			cbMeshBuffer.gWorldViewProj = mesh->worldTM * RenderManager::s_cameraInfo.viewTM * RenderManager::s_cameraInfo.projTM;
			cbMeshBuffer.objectID = mesh->materialID;
			cbOcean cbOceanBuffer;
			cbOceanBuffer.waterTranslation = _waterTranslation;
			cbWave cbWaveBuffer;
			cbWaveBuffer.WaveTime.y = _waveTime;	// deltatime

			// 파동이 움직이는것을 조절
			_waterTranslation += 0.01f;

			if (_waterTranslation > 100)
			{
				_waterTranslation = 0.f;
			}

			cbReflection cbReflectionBuffer;
			cbReflectionBuffer.gReflection = ReflectionSkyPass::reflectionView;	// ?

			cbWater cbWaterBuffer;

			Vector4 refractionTint = Vector4(0.0f, 0.8f, 1.0f, 1.0f);
			cbWaterBuffer.refractionTint = refractionTint;
			cbWaterBuffer.reflectRefractScale = 0.08f;	// 물 법선 맵의 스케일링 값을 설정 // 하늘이 일그러지는수치를 의미
			cbWaterBuffer.uvScale = _wateruvScale;	// 수질 변환

			// 하늘이 흘러가는 수치를 의미
			_wateruvScale += 0.00005f;

			if (_wateruvScale > 50.0f)
			{
				_wateruvScale -= 0.00005f;
			}

			g_deviceContext->RSSetState(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetRasterState().Get());
			_OceanTest_VS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			//_OceanTest_VS->ConstantBufferUpdate(&cbWaveBuffer, "cbWave");
			_OceanTest_VS->ConstantBufferUpdate(&cbReflectionBuffer, "cbReflection");


			_OceanTest_VS->Update();

			_OceanTest_PS->ConstantBufferUpdate(&cbOceanBuffer, "cbOcean");
			_OceanTest_PS->ConstantBufferUpdate(&cbMeshBuffer, "cbMesh");
			_OceanTest_PS->ConstantBufferUpdate(&cbWaterBuffer, "cbWater");
			_OceanTest_PS->ConstantBufferUpdate(&ResourceManager::cbMaterialBuffer, "cbMaterial");

			//_OceanTest_PS->SetResourceViewBuffer(_OceanReflectionSRV, "reflectionTexture");
			_OceanTest_PS->SetResourceViewBuffer(ResourceManager::GetInstance()->materials[mesh->materialID]->normalTexture, "ReflectionNomalMap");

			_OceanTest_PS->SetResourceViewBuffer(_gReflectionSkySRV, "ReflectionSkyTexture");
			_OceanTest_PS->SetResourceViewBuffer(_gReflectionObjectSRV, "ReflectionObjectTexture");
			_OceanTest_PS->Update();

			// Set vertex buffer stride and offset
			unsigned int stride = ResourceManager::GetInstance()->GetMesh(mesh->name)->stride;
			unsigned int offset = 0;

			// 어떤 방식으로 그릴지
			g_deviceContext->IASetPrimitiveTopology(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetPrimitiveTopology());

			// 버텍스 버퍼 설정
			g_deviceContext->IASetVertexBuffers(0, 1, ResourceManager::GetInstance()->GetMesh(mesh->name)->GetVertexBuffer().GetAddressOf(), &stride, &offset);

			// 인덱스 버퍼 설정
			g_deviceContext->IASetIndexBuffer(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

			// 그린다!
			//if (!mesh->isBone)
			g_deviceContext->DrawIndexed(ResourceManager::GetInstance()->GetMesh(mesh->name)->GetIdxBufferSize(), 0, 0);
		}
		break;
		case OBJECT_TYPE::WaveEffect:
		{
			continue;
		}
		break;
		}
	}
}


