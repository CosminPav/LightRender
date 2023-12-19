
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
	float3 normal: NORMAL0;
	float3 world_pos: TEXCOORD1;
};

cbuffer Constants : register(b0)
{
	row_major float4x4 World;
	row_major float4x4 View;
	row_major float4x4 Projection;

	float4 LightDirection;
	float4 CameraPosition;

	float4 LightPosition;
	float LightRadius;

	float Time;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//World space
	output.pos = mul(input.pos, World);

	output.world_pos = output.pos.xyz;


	//View space
	output.pos = mul(output.pos, View);

	//Screen space
	output.pos = mul(output.pos, Projection);


	output.texcoord = input.texcoord;
	output.normal = input.normal;
	return output;
}