#include "pch.h"
#include "VertexShader.h"
#include "GraphicsEngineAPI.h"
#include "Device.h"
#include "ShaderResource.h"

VertexShader::VertexShader(const wstring& shaderName, const wstring& path, string entryPoint, const D3D_SHADER_MACRO* macro)
	: ShaderBase(SHADER_TYPE::VERTEX, shaderName),
	_vs(nullptr), _inputLayout(nullptr)
{
	CreateShader(path + shaderName, entryPoint, macro);		// ��ü�� �����ɶ� ���̴��� �����.
}

VertexShader::~VertexShader()
{}

void VertexShader::CreateShader(const wstring& path, string entryPoint, const D3D_SHADER_MACRO* macro)
{
	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	ID3D11ShaderReflection* pReflection = nullptr;
	vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescArray;		// inputlayout��ҵ��� �ӽ������� ����

	DWORD shaderFlags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		// ��ġ�ؼ� �־���
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	// ���� ���̴��ڵ带 ������ �Ѵ�.
	HRESULT hr = D3DCompileFromFile(path.c_str(), macro, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), "vs_5_0", shaderFlags, NULL, &compiledShader, &compilationMsgs);
	
	if (FAILED(hr))
		MessageBox(0, L"vertexshader Failed.", 0, 0);
	
	if (compilationMsgs != 0)
		compilationMsgs->Release();

	// vertex shader ����
	GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice()->
		CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, _vs.GetAddressOf());
	
	// reflection ����
	D3DReflect(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflection);

	D3D11_SHADER_DESC shaderDesc;			// �ش� ����ü�� ���� ShaderReflection���� Shader ������ �����´�.
	pReflection->GetDesc(&shaderDesc);		

	for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pReflection->GetInputParameterDesc(i, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC inputElementDesc;
		inputElementDesc.SemanticName = paramDesc.SemanticName;
		inputElementDesc.SemanticIndex = paramDesc.SemanticIndex;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputElementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputElementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputElementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		inputElementDesc.InputSlot = 0;
		inputElementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputElementDesc.InstanceDataStepRate = 0;

		inputElementDescArray.push_back(inputElementDesc);
	}

	// inputLayout ����
	GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice()->
		CreateInputLayout(&inputElementDescArray[0], (UINT)inputElementDescArray.size(), compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), _inputLayout.GetAddressOf());

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

void VertexShader::Update()
{
	// VS�� vertex shader ����
	GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()->
		VSSetShader(_vs.Get(), nullptr, 0);

	// VS�� ��� ���� ����
	if (!_constantBufferData.empty())
	{
		GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()->
			VSSetConstantBuffers(0, (UINT)_constantBuffers.size(), _constantBuffers[0].GetAddressOf());
	}
	
	// VS�� ���÷� ����
	if (!_samplerStateData.empty())
	{
		GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()->
			VSSetSamplers(0, (UINT)_samplerStates.size(), _samplerStates[0].GetAddressOf());
	}

	// VS�� ���ҽ� ����
	if (!_shaderResourceViewData.empty())
	{
		GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()->
			VSSetShaderResources(0, (UINT)_shaderResourceViews.size(), _shaderResourceViews[0].GetAddressOf());
	}

	// inputLayout ����
	GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()->
		IASetInputLayout(_inputLayout.Get());
}

void VertexShader::Release()
{
	_vs->Release();
	_inputLayout->Release();
}
