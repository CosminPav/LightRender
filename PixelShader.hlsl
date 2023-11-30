
Texture2D EarthColor: register(t0);
sampler EarthColorSampler : register(s0);

Texture2D EarthSpecular : register(t1);
sampler EarthSpecularSampler : register(s1);

Texture2D CloudsTexture : register(t2);
sampler CloudsSpecularSampler : register(s2);

Texture2D EarthColor_Night : register(t3);
sampler EarthColor_NightSampler : register(s3);

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
	float Time;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 earthColor = EarthColor.Sample(EarthColorSampler, 1.0 - input.texcoord);		//color map
	float earthSpecular = EarthSpecular.Sample(EarthSpecularSampler, 1.0 - input.texcoord).r;	//specular map

	float cloudsTexture = CloudsTexture.Sample(CloudsSpecularSampler, 1.0 - input.texcoord + float2(Time/100.0, 0)).r;	//clouds map

	float4 earthColor_night = EarthColor_Night.Sample(EarthColor_NightSampler, 1.0 - input.texcoord);	//night color map

	/** AMBIENT LIGHT */
	float Ka = 1.5;	//Ambient reflection factor
	float3 Ia = float3(0.09, 0.082, 0.082);	//Ambient light color
	Ia *= earthColor.rgb ;
	float3 AmbientLight = Ka * Ia;	//Ambient light

	/** DIFFUSE LIGHT */
	float Kd = 0.7f;	//Diffuse reflection factor

	//Day
	float3 Id_Day = float3(1.0f, 1.0f, 1.0f);	//Diffuse light color
	Id_Day *= (earthColor.rgb + cloudsTexture);

	//Night
	float3 Id_Night = float3(1.0f, 1.0f, 1.0f);
	Id_Night *= (earthColor_night.rgb + cloudsTexture);

	float AmountDiffuseLight = dot(LightDirection.xyz, input.normal);

	//Lerp between day and night color maps
	float3 Id = lerp(Id_Night, Id_Day, (AmountDiffuseLight + 1.0) / 2.0);

	float3 DiffuseLight = Kd * Id;  //Diffuse light

	/** SPECULAR LIGHT */
	float Ks = earthSpecular;			//Specular reflection factor
	float3 Is = float3(1.0f, 1.0f, 1.0f);		//Specular light color

	float3 ReflectedLight = reflect(LightDirection.xyz, input.normal);
	float Shininess = 100.0f;
	float AmountSpecularLight = pow(max(0.0, dot(ReflectedLight, input.dir_to_cam)), Shininess);

	float3 SpecularLight = Ks * AmountSpecularLight * Is;	//Specular light

	
	float3 Light = AmbientLight + DiffuseLight + SpecularLight;

	return float4(Light, 1.0f);
}