
Texture2D Texture: register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
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
	float4 LightDirection;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	/** AMBIENT LIGHT */
	float Ka = 0.1f;	//Ambient reflection factor
	float3 Ia = float3(1.0f, 1.0f, 1.0f);	//Ambient light color
	float3 AmbientLight = Ka * Ia;	//Ambient light

	/** DIFFUSE LIGHT */
	float Kd = 0.7f;	//Diffuse reflection factor
	float3 Id = float3(1.0f, 1.0f, 1.0f);	//Diffuse light color
	float AmountDiffuseLight = max(0.0, dot(LightDirection.xyz, input.normal));
	float3 DiffuseLight = Kd * AmountDiffuseLight * Id;  //Diffuse light

	/** SPECULAR LIGHT */
	float Ks = 1.0f;			//Specular reflection factor
	float3 Is = float3(1.0f, 1.0f, 1.0f);		//Specular light color

	float3 ReflectedLight = reflect(LightDirection.xyz, input.normal);
	float Shininess = 100.0f;
	float AmountSpecularLight = pow(max(0.0, dot(ReflectedLight, input.dir_to_cam)), Shininess);

	float3 SpecularLight = Ks * AmountSpecularLight * Is;	//Specular light

	
	float3 Light = AmbientLight + DiffuseLight + SpecularLight;

	return float4(Light, 1.0f);
}