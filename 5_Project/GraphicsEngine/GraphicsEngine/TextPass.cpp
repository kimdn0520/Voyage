#include "pch.h"
#include "TextPass.h"
#include "RenderManager.h"
#include "GraphicsEngineAPI.h"
#include "Device.h"
#include "DepthStencilState.h"

TextPass::TextPass()
{}

TextPass::~TextPass()
{}

void TextPass::Init()
{
	g_device = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDevice();
	g_deviceContext = GraphicsEngineAPI::GetInstance()->GetDeviceClass()->GetDeviceContext();

	_spriteBatch = make_shared<DirectX::SpriteBatch>(g_deviceContext.Get());

	TCHAR* fileName = (TCHAR*)L"Data/Font/SB_M.spritefont";

	// 동적으로 바꾸려면.. 얘를 여러개 만들어서 써야할듯..?,,
	_spriteFont = make_shared<DirectX::SpriteFont>(g_device.Get(), fileName);

	_spriteFont->SetLineSpacing(20.0f);
}

void TextPass::Release()
{
	_spriteBatch.reset();
}

void TextPass::DrawTextColor(int x, int y, Vector4 paColor, TCHAR* paText, float paScale ...)
{
	TCHAR _buffer[1024] = L"";
	va_list vl;
	va_start(vl, paText);
	_vstprintf(_buffer, 1024, paText, vl);
	va_end(vl);

	_spriteBatch->Begin();
	//_spriteFont->DrawString(_spriteBatch.get(), _buffer, DirectX::XMFLOAT2((float)x, (float)y), paColor);
	_spriteFont->DrawString(_spriteBatch.get(), _buffer, DirectX::XMFLOAT2((float)x, (float)y), paColor, 0,
		DirectX::XMFLOAT2((float)0, (float)0), paScale);
	_spriteBatch->End();
}

void TextPass::RenderText(vector<shared_ptr<TextInfo>> texts)
{
	for (auto& text : texts)
	{
		char* strID;
		size_t tempSize = text->text.length();
		strID = new char[tempSize];
		strcpy(strID, text->text.c_str());

		// char* to TCHAR*
		TCHAR szUniID[256] = { 0, };
		int len = strlen(strID);
		::MultiByteToWideChar(CP_ACP, 0, strID, -1, szUniID, len + 1);

		DrawTextColor(text->x, text->y, text->color, szUniID, text->scale);
	}

	g_deviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
	g_deviceContext->OMSetDepthStencilState(GraphicsEngineAPI::GetInstance()->GetDepthStencilStateClass()->GetDepthStencilState().Get(), 1);
}

