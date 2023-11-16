
struct VS_INPUT
{
	float4 pos : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal: NORMAL0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal: TEXCOORD1;
	float3 dir_to_cam: TEXCOORD2;
};

cbuffer Constants : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;

	float4 CameraPosition;
	float4 LightDirection;
};

VS_OUTPUT vsmain(VS_INPUT input )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//convert to world space
	//output.pos = lerp(input.pos, input.pos1, (sin(mAngle) + 1.0f) / 2.0f);
	 
	//World space
	output.pos = mul(input.pos, World);

	//compute the direction to camera vector
	output.dir_to_cam = normalize(output.pos.xyz - CameraPosition.xyz);

	//View space
	output.pos = mul(output.pos, View);

	//Screen space
	output.pos = mul(output.pos, Projection);


	output.texcoord = input.texcoord;
	output.normal = input.normal;
	return output;
}