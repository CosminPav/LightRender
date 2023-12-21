
struct VS_INPUT
{
	float4 pos : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal: NORMAL0;
	float3 tangent : TANGENT0;
	float3 bitangent : BITANGENT0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 dir_to_cam: TEXCOORD1;
	row_major float3x3 TBN:TEXCOORD2;
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

	output.TBN[0] = normalize(mul(input.tangent, World));
	output.TBN[1] = normalize(mul(input.bitangent, World));
	output.TBN[2] = normalize(mul(input.normal, World));

	return output;
}