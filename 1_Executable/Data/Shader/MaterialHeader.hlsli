#define Material_Count_ID 50

struct MaterialCollection
{
	float4 gMaterialAmbient;
	float4 gMaterialDiffuse;
	float4 gMaterialSpecular;
	float4 gMaterialReflection;

	bool isDiffuseTexture;			// hlsl���� boolŸ���� 4byte �̹Ƿ� �Ϸ� ����ü �Ѱ��ٶ� BOOL�� �Ѱ�����Ѵ�
	bool isNormalTexture;
	bool isSpecularTexture;
	bool isReflectionTexture;
	bool isUtil;					// Light �޴��� �ȹ޴��� üũ
};