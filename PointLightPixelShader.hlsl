
Texture2D TextureColor: register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
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

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 textureColor = TextureColor.Sample(TextureSampler, (1.0 - input.texcoord));		//color map

	/** AMBIENT LIGHT */
	float Ka = 1.5;	//Ambient reflection factor
	float3 Ia = float3(0.09, 0.082, 0.082);	//Ambient light color
	Ia *= textureColor.rgb;
	float3 AmbientLight = Ka * Ia;	//Ambient light

	/** DIFFUSE LIGHT */
	float Kd = 0.7f;	//Diffuse reflection factor
	float3 light_direction = normalize(LightPosition.xyz - input.world_pos.xyz);

	float DistanceLightObject = length(LightPosition.xyz - input.world_pos.xyz); //calc the distance to from the light source to the object

	float FadeArea =max(0, DistanceLightObject - LightRadius); //the fade area

	//linear quadratic attenuation
	float ConstantFunc = 1.0;	//constant attenuation
	float LinearFunc = 2.0;	//linear attenuation
	float QuadraticFunc = 2.0;	//quadratic attenuation

	float Attenuation = ConstantFunc + LinearFunc * FadeArea + QuadraticFunc * FadeArea * FadeArea;

	//light attenuation(basci formula)
	//float Attenuation = 1.0 + FadeArea;	

	float AmountDiffuseLight = max(0,dot(light_direction.xyz, input.normal));
	float3 Id = float3(1.0, 1.0, 1.0);
	Id *= textureColor.rgb;

	float3 DiffuseLight =( Kd * Id * AmountDiffuseLight) / Attenuation;  //Diffuse light

	/** SPECULAR LIGHT */
	float Ks = 1.0;			//Specular reflection factor
	float3  dir_to_cam = normalize(input.world_pos.xyz - CameraPosition.xyz);
	float3 Is = float3(1.0f, 1.0f, 1.0f);		//Specular light color

	float3 ReflectedLight = reflect(light_direction.xyz, input.normal);
	float Shininess = 30.0f;
	float AmountSpecularLight = pow(max(0.0, dot(ReflectedLight, dir_to_cam)), Shininess);

	float3 SpecularLight = (Ks * AmountSpecularLight * Is)/ Attenuation;	//Specular light


	float3 Light = AmbientLight + DiffuseLight + SpecularLight;

	return float4(Light, 1.0f);
}