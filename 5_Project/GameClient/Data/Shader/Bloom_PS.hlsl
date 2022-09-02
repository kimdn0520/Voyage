#define BLOOMCURVE_METHOD_1
//#define BLOOMCURVE_METHOD_2
//#define BLOOMCURVE_METHOD_3

float GetBloomCurve(float x, float threshold)
{
	float result = x;
	x *= 2.0f;

#ifdef BLOOMCURVE_METHOD_1
	result = x * 0.05f + max(0, x - threshold) * 0.5f;	// default threshold = 1.26
#endif

#ifdef BLOOMCURVE_METHOD_2
	result = x * x / 3.2f;	// default threshold = 1.26
#endif

#ifdef BLOOMCURVE_METHOD_3
	result = max(0, x - threshold);	// default threshold = 1.0
	result += result;
#endif

	return result * 0.5f;
}


float4 Bloom_PS(float x, float float threshold) : SV_TARGET
{


	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}