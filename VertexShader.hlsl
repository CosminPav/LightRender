
struct VS_INPUT
{
	float4 pos : POSITION;
	float3 color : COLOR;
	float3 color1 : COLOR1;
};

struct VS_OUTPUT
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

VS_OUTPUT vsmain(VS_INPUT input )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//convert to world space
	//output.pos = lerp(input.pos, input.pos1, (sin(mAngle) + 1.0f) / 2.0f);
	 
	//World space
	output.pos = mul(input.pos, World);

	//View space
	output.pos = mul(output.pos, View);

	//Screen space
	output.pos = mul(output.pos, Projection);


	output.color = input.color;
	output.color1 = input.color1;
	return output;
}