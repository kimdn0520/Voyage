#define Material_Count_ID 50

struct MaterialCollection
{
	float4 gMaterialAmbient;
	float4 gMaterialDiffuse;
	float4 gMaterialSpecular;
	float4 gMaterialReflection;

	bool isDiffuseTexture;			// hlsl에서 bool타입은 4byte 이므로 일로 구조체 넘겨줄때 BOOL로 넘겨줘야한대
	bool isNormalTexture;
	bool isSpecularTexture;
	bool isReflectionTexture;
	bool isUtil;					// Light 받는지 안받는지 체크
};