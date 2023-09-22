
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float3 color1 : COLOR1;
};

cbuffer Constants : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;
	unsigned int mAngle;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	return float4(lerp(input.color, input.color1, (float)((sin((float)(mAngle / (float)500.0f)) + 1.0f) / 2.0f)),1.0f);
	//return float4(lerp(input.color, input.color1, (sin(mAngle) + 1) / 2), 1);
}