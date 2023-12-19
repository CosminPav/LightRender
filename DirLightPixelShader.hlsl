
Texture2D Color: register(t0);
sampler ColorSampler : register(s0);


struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal: NORMAL0;
	float3 dir_to_cam: TEXCOORD1;
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
	float4 colorMap = Color.Sample(ColorSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));		//color map
	
	/** AMBIENT LIGHT */
	float Ka = 1.5;	//Ambient reflection factor
	float3 Ia = float3(0.09, 0.082, 0.082);	//Ambient light color
	Ia *= colorMap.rgb;
	float3 AmbientLight = Ka * Ia;	//Ambient light

	/** DIFFUSE LIGHT */
	float Kd = 0.7f;	//Diffuse reflection factor

	float AmountDiffuseLight = dot(LightDirection.xyz, input.normal);

	//Lerp between day and night color maps
	float3 Id = float3(1.0, 1.0, 1.0);//lerp(Id_Night, Id_Day, (AmountDiffuseLight + 1.0) / 2.0);
	Id *= colorMap.rgb;
	float3 DiffuseLight = Kd * Id * AmountDiffuseLight;  //Diffuse light

	/** SPECULAR LIGHT */
	float Ks = 0.0;			//Specular reflection factor
	float3 Is = float3(1.0f, 1.0f, 1.0f);		//Specular light color

	float3 ReflectedLight = reflect(LightDirection.xyz, input.normal);
	float Shininess = 100.0f;
	float AmountSpecularLight = pow(max(0.0, dot(ReflectedLight, input.dir_to_cam)), Shininess);

	float3 SpecularLight = Ks * AmountSpecularLight * Is;	//Specular light


	float3 Light = AmbientLight + DiffuseLight + SpecularLight;

	return float4(Light, 1.0f);
}