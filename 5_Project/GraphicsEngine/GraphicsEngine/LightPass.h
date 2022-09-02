//#pragma once
//#include "PassBase.h"
//
//class VertexShader;
//class PixelShader;
//
///// <summary>
///// TODO : Deferred ·Î ¹Ù²Ü ¿¹Á¤
///// </summary>
//class LightPass : public PassBase
//{
//public:
//	LightPass();
//	~LightPass();
//
//private:
//	shared_ptr<VertexShader> _basic_VS;
//	shared_ptr<PixelShader> _basic_PS;
//
//	shared_ptr<VertexShader> _light_VS;
//	shared_ptr<VertexShader> _light_VS_1;
//	shared_ptr<PixelShader> _light_PS;
//
//	shared_ptr<VertexShader> _skyBox_VS;
//	shared_ptr<PixelShader> _skyBox_PS;
//
//public:
//	void Init() override;
//
//	void Render(vector<shared_ptr<MeshInfo>> meshs) override;
//};
//
