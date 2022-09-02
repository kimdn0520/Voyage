#include "pch.h"
#include "Device.h"

Device::Device()
	: _device(nullptr), _deviceContext(nullptr)
{}

Device::~Device()
{}

void Device::Init()
{
	D3D_FEATURE_LEVEL featureLevel;						// ���� ����
	UINT createDeviceFlags = 0;									// �÷���
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;				// ����� ��� ���忡�� ����� ������ Ȱ��ȭ�ϴ� �÷���
#endif

	// device ����
	HRESULT hResult = D3D11CreateDevice(
		0,																			// �⺻ ����� 
		D3D_DRIVER_TYPE_HARDWARE,						// 3���� �׷��� ���� ����
		0,																			// ����Ʈ���� ��ġ�� ������� ����
		createDeviceFlags,												//  �߰����� ��ġ ���� �÷��׵�
		0, 0,																	    // default feature level array - �ΰ��� �����ϸ� �����Ǵ� �ְ� ��� ������ ���õȴ�.
		D3D11_SDK_VERSION,
		_device.GetAddressOf(),
		&featureLevel,
		_deviceContext.GetAddressOf());

	if (FAILED(hResult))
		return;

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
		return;
		
	// 4X MSAA ǰ�� ���� ���� ����
	_device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMsaaQuality);
	assert(_4xMsaaQuality > 0);			// 4X MSAA�� �׻� �����ǹǷ�, ��ȯ�� ǰ�� ���� ���� �ݵ�� 0���� Ŀ�� �Ѵ�.
}

void Device::Release()
{
	_device.ReleaseAndGetAddressOf();
	_deviceContext.ReleaseAndGetAddressOf();
}
