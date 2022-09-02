#pragma once
#include "PassBase.h"

class TextPass : public PassBase
{
public:
	TextPass();
	~TextPass();

private:
	shared_ptr<SpriteFont> _spriteFont;
	shared_ptr<SpriteBatch> _spriteBatch;

	// font�� �������� �ٲٴ¹� ������..
	wstring _fontFilePath;
	wstring _extension;

public:
	void Init() override;
	
	void Release();

	void DrawTextColor(int x, int y, Vector4 color, TCHAR* text, float paScale ...);

	void RenderText(vector<shared_ptr<TextInfo>> meshs);
};

