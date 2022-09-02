#include "pch.h"
#include "Texture.h"
#include "DDSTextureLoader.h"
#include "GraphicsEngineAPI.h"
#include "Device.h"

Texture::Texture()
    : _mapSRV(nullptr), _image(ScratchImage()), _texResource(nullptr)
{}

Texture::~Texture()
{}

void Texture::Init(const wstring& path)
{
    /*HRESULT hr = CreateDDSTextureFromFile(GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice().Get(),
       path.c_str(),
       &texResource,
       _mapSRV.GetAddressOf());*/

    std::wstring ext = fs::path(path).extension();

    if (ext == L".dds" || ext == L".DDS")
        ::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, _image);
    else// png, jpg, jpeg, bmp
        ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, _image);

    HRESULT hr = ::CreateTexture(GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice().Get(), _image.GetImages(), _image.GetImageCount(), _image.GetMetadata(),
        reinterpret_cast<ID3D11Resource**>(_texResource.GetAddressOf()));

    if (FAILED(hr))
        MessageBox(0, L"Texture Failed.", 0, 0);

    bool isCubeMap = _image.GetMetadata().IsCubemap();

    /*if (path.find(L"cube") != std::wstring::npos)
    {
        isCubeMap = true;
    }*/
    
    D3D11_TEXTURE2D_DESC textureDesc;

    _texResource->GetDesc(&textureDesc);

    _width = textureDesc.Width;

    _height = textureDesc.Height;

    _levels = textureDesc.MipLevels;

    _format = textureDesc.Format;

    // 레벨 부여
    //if (_levels <= 0)
        //_levels = Texture::NumMipmapLevels(_width, _height);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

    srvDesc.Format = textureDesc.Format;
    srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;

    if (!isCubeMap)
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    else
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;

    hr = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice()->CreateShaderResourceView(_texResource.Get(), &srvDesc, _mapSRV.GetAddressOf());

	if (FAILED(hr))
		MessageBox(0, L"Texture Failed.", 0, 0);

    // Mip Level이 만약 0이면 .. Mip Map 형성해주자.
    if (textureDesc.MipLevels == 0)
        GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext()->GenerateMips(this->_mapSRV.Get());
}