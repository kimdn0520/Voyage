#include "pch.h"
#include "PixelShader.h"
#include "GraphicsEngineAPI.h"
#include "Device.h"
#include "ShaderResource.h"

PixelShader::PixelShader(const wstring& shaderName, const wstring& path, string entryPoint, const D3D_SHADER_MACRO* macro)
	: ShaderBase(SHADER_TYPE::PIXEL, shaderName)
{
	CreateShader(path + shaderName, entryPoint, macro);		// ��ü�� �����ɶ� ���̴��� �����.
}

PixelShader::~PixelShader()
{}

void PixelShader::CreateShader(const wstring & path, string entryPoint, const D3D_SHADER_MACRO* macro)
{
	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	ID3D11ShaderReflection* pReflection = nullptr;

	DWORD shaderFlags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(path.c_str(), macro, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), "ps_5_0", shaderFlags, NULL, &compiledShader, &compilationMsgs);

	if (FAILED(hr))
		MessageBox(0, L"pixelshader Failed.", 0, 0);

	if (compilationMsgs != 0)
		compilationMsgs->Release();

	// pixel shader ����
	GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice()->
		CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, _ps.GetAddressOf());

	// reflection ����
	D3DReflect(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflection);

	D3D11_SHADER_DESC shaderDesc;
	pReflection->GetDesc(&shaderDesc);

	// Cbuffer, Sampler, Texture Data ����
	// ��.. �ش� ����ü�� name, bindpoint(register slot), size �����͵��� �����ؼ� ����صΰ��ִ´�..
	int cbufferRegisterSlot = 0;
	int srvRegisterSlot = 0;
	int samplerRegisterSlot = 0;

	for (int i = 0; i < shaderDesc.BoundResources; i++)
	{
		// �ش� struct�� ���� binding�� resource name�� �������� ������ �����´�.
		D3D11_SHADER_INPUT_BIND_DESC resource_desc;
		pReflection->GetResourceBindingDesc(i, &resource_desc);

		switch (resource_desc.Type)
		{
			case D3D_SIT_CBUFFER:
			{
				ID3D11ShaderReflectionConstantBuffer* cBuffer = pReflection->GetConstantBufferByName(resource_desc.Name);
				D3D11_SHADER_BUFFER_DESC cbDesc;
				cBuffer->GetDesc(&cbDesc);

				cbufferRegisterSlot = resource_desc.BindPoint;

				_constantBufferData[resource_desc.Name] = make_shared<ConstantBuffer>(resource_desc.Name, cbufferRegisterSlot, cbDesc.Size);
			}
			break;
			case D3D_SIT_TEXTURE:
			{
				srvRegisterSlot = resource_desc.BindPoint;

				_shaderResourceViewData[resource_desc.Name] = make_shared<ShaderResourceBuffer>(resource_desc.Name, srvRegisterSlot);
			}
			break;
			case D3D_SIT_SAMPLER:
			{
				samplerRegisterSlot = resource_desc.BindPoint;

				_samplerStateData[resource_desc.Name] = make_shared<SamplerBuffer>(resource_desc.Name, samplerRegisterSlot);
			}
			break;
			default:
				break;
		}
	}

	// ������ ����
	_constantBuffers.resize(++cbufferRegisterSlot);
	_shaderResourceViews.resize(++srvRegisterSlot);
	_samplerStates.resize(++samplerRegisterSlot);

	// ShaderReflection ����
	pReflection->Release();
}

void PixelShader::Update()
{
	// PS�� pixel shader ����
	GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()->
		PSSetShader(_ps.Get(), nullptr, 0);

	// PS�� ��� ���� ����
	if (!_constantBufferData.empty())
	{
		GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()->
			PSSetConstantBuffers(0, (UINT)_constantBuffers.size(), _constantBuffers[0].GetAddressOf());
	}
	
	// PS�� ���÷� ����
	if (!_samplerStateData.empty())
	{
		GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()->
			PSSetSamplers(0, (UINT)_samplerStates.size(), _samplerStates[0].GetAddressOf());
	}

	// PS�� ���ҽ� ����
	if (!_shaderResourceViews.empty())
	{
		GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()->
			PSSetShaderResources(0, (UINT)_shaderResourceViews.size(), _shaderResourceViews[0].GetAddressOf());
	}
}

void PixelShader::Release()
{
	_ps->Release();
}
