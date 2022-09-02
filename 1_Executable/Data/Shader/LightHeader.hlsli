
#define DIRECTION_LIGHT_COUNT	1
#define POINT_LIGHT_COUNT		30
#define SPOT_LIGHT_COUNT		30

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	// Packed into 4D vector: (Position, Range)
	float3 Position;
	float Range;

	// Packed into 4D vector: (A0, A1, A2, Pad)
	float3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	// Packed into 4D vector: (Position, Range)
	float3 Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	float3 Direction;
	float Spot;

	// Packed into 4D vector: (Att, Pad)
	float3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

//---------------------------------------------------------------------------------------
// ������ HLSL �Լ��� �־��� ������ ���Ɽ��, ǥ�� ����, �׸��� ���� ����(���� �޴� ǥ��
// ������ ������ ���� ���� ����)�� �����ؼ� ǥ�� ���� ���� ��(�ݻ�� ���� ����)��
// ����Ѵ�.
//---------------------------------------------------------------------------------------
void ComputeDirectionalLight(float4 Ambient, float4 Diffuse, float4 Specular, float4 Reflect,
	DirectionalLight L,
	float3 normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	// �� ���ʹ� ������ ���� �ݴ� �������� ���ش�.
	float3 lightVec = -L.Direction;

	// �ֺ��� ���� ���� * �Ի� �ֺ���
	ambient = Ambient * L.Ambient;

	// L*n ���ִ� �κ� [���� ����]!! 
	float diffuseFactor = dot(lightVec, normal);

	// �׷��� �� �������ذ� 0���� ũ�� (����̸�) ���� ���Ⱑ ����.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 r = reflect(-lightVec, normal);			// specular�� ����~  �ݻ� ����?
		// �ݻ纤�Ͷ� �������Ͷ� �����ϰ� �ݿ� ���� p(Specular.w)�� �����Ͽ� �ŵ� ����..
		float specFactor = pow(max(dot(r, toEye), 0.0f), Specular.w);

		// �� ~ ������Ʈ�ڻ��� ��Ģ * �л걤 ���� ���� * �Ի� �л걤 ���ִ� �κ��ΰͰ���~
		diffuse = diffuseFactor * Diffuse * L.Diffuse;

		spec = specFactor * Specular * L.Specular;
	}
}

//---------------------------------------------------------------------------------------
// ������ HLSL �Լ��� �־��� ������ ������, ǥ�� �� ��ġ, ǥ�� ����, �׸��� ���� ����(��
// �� �޴� ǥ�� ������ ������ ���� ���� ����)�� �����ؼ� ǥ�� ���� ���� ��(�ݻ�� ���� ��
// ��)�� ����Ѵ�.
//---------------------------------------------------------------------------------------
void ComputePointLight(float4 Ambient, float4 Diffuse, float4 Specular, float4 Reflection, PointLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// ��� ���е��� �ʱ�ȭ�Ѵ�.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// �� ����(ǥ�� ������ ���������� ����)
	float3 lightVec = L.Position - pos;

	// ǥ�� ���� ���� ������ �Ÿ�
	float d = length(lightVec);

	// ���� ����
	if (d > L.Range)
		return;

	// �� ���͸� ����ȭ �Ѵ�.
	lightVec /= d;

	// �ֺ��� ��
	ambient = Ambient * L.Ambient;

	// ���� ������ �ʰ� ǥ�鿡 �����Ѵٴ� ���� �Ͽ���
	// �л� �װ� �ݿ��� ���� ���Ѵ�.

	float diffuseFactor = dot(lightVec, normal);

	// ���� �б⸦ ���ϱ� ���� ���ǹ��� ��ģ��.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 r = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(r, toEye), 0.0f), Specular.w);

		diffuse = diffuseFactor * Diffuse * L.Diffuse;
		spec = specFactor * Specular * L.Specular;
	}

	// ����
	float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));

	diffuse *= att;
	spec *= att;
}

//---------------------------------------------------------------------------------------
// ������ HLSL �Լ��� �־��� ������ ��������, ǥ�� �� ��ġ, ǥ�� ����, �׸��� ���� ����
// (���� �޴� ǥ�� ������ ������ ���� ��������)�� �����ؼ� ǥ�� ���� ���� ��(�ݻ�� ����
// ����)�� ����Ѵ�.
//---------------------------------------------------------------------------------------
void ComputeSpotLight(float4 Ambient, float4 Diffuse, float4 Specular, float4 Reflection, SpotLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// ��� ���е��� �ʱ�ȭ�Ѵ�.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// �� ����(ǥ�� ������ ���������� ����)
	float3 lightVec = L.Position - pos;

	// ǥ�� ���� ���� ������ �Ÿ�
	float d = length(lightVec);

	// ���� ����
	if (d > L.Range)
		return;

	// �� ���͸� ����ȭ�Ѵ�.
	lightVec /= d;

	// �ֺ��� ��
	ambient = Ambient * L.Ambient;

	// ���� ������ �ʰ� ǥ�鿡 �����Ѵٴ� ���� �Ͽ���
	// �л걤 �װ� �ݿ��� ���� ���Ѵ�.

	float diffuseFactor = dot(lightVec, normal);

	// ���� �б⸦ ���ϱ� ���� ���ǹ��� ��ģ��.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 r = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(r, toEye), 0.0f), Specular.w);

		diffuse = diffuseFactor * Diffuse * L.Diffuse;
		spec = specFactor * Specular * L.Specular;
	}

	// ������ ����� ����Ѵ�.
	// L.Spot�� ������ ������ Ű��ų� ���� �� �ִ�.
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

	// ���� ���⸦ ������ ����� ����ϰ� �Ÿ��� ���� �����Ų��.
	float att = spot / dot(L.Att, float3(1.0f, d, d * d));

	ambient *= spot;
	diffuse *= att;
	spec *= att;
}

//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	// Build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N) * N);	
	//float3 T = tangentW;
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}