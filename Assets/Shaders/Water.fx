//Description: Simple shader that does applies transformation and lighting

cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix WVP; // world view projection - all three matricies combined . local - camera 
	float3 ViewPosition;
}

cbuffer LightBuffer : register(b1)
{
	float3 LightDirection;
	float4 LightAmbient;
	float4 LightDiffuse;
	float4 LightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
	float4 MaterialAmbient;
	float4 MaterialDiffuse;
	float4 MaterialSpecular;
	float MaterialPower; //shininess
}

cbuffer SettingsBuffer : register(b3)
{
	float specularMapWeight : packoffset(c0.x);
	float bumpMapWeight : packoffset(c0.y);
	float normalMapWeight : packoffset(c0.z);
	float aoWeight : packoffset(c0.w);
	float brightness : packoffset(c1.x);
	bool useShadow : packoffset(c1.y);
	float depthBias : packoffset(c1.z);
	float movement : packoffset(c1.w);
	float movementSpeed : packoffset(c2.x);
}
cbuffer ShadowBuffer : register(b4)
{
	matrix WVPLight;
}

//cbuffer ClippingBuffer : register(b5)
//{
//	float4 clippingPlane;
//}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D aoMap : register(t4);
Texture2D depthMap : register(t5);
Texture2D refractionMap : register(t6);
Texture2D reflectionMap : register(t7);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 worldNormal : NORMAL;
	float3 worldTangent : TEXCOORD0;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
	float2 texCoord : TEXCOORD3;
	float4 positionNDC : TEXCOORD4;
	float4 positionScreen : TEXCOORD5;
	//float clip : SV_ClipDistance0;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	//float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float xCoord = input.texCoord.x + movement;
	if (xCoord > 1.0f)
		xCoord -= 1.0f;
	float2 displacement = displacementMap.SampleLevel(textureSampler, float2(xCoord, input.texCoord.y), 1).rg * 2.0f - 1.0f;
	float3 localPosition = input.position + (input.normal * float3(displacement, 1.0f) * bumpMapWeight);
	float3 worldPosition = mul(float4(localPosition, 1.0f), World).xyz;
	float3 worldNormal = mul(input.normal, (float3x3) World);
	float3 worldTangent = mul(input.tangent, (float3x3) World);
	
	output.position = mul(float4(localPosition, 1.0f), WVP);
	output.positionScreen = mul(float4(input.position, 1.0f), WVP);
	
	output.worldNormal = worldNormal;
	output.worldTangent = worldTangent;
	output.dirToLight = -LightDirection;
	output.dirToView = normalize(ViewPosition - worldPosition);
	output.texCoord = input.texCoord;

	//if (useShadow)
	//	output.positionNDC = mul(float4(localPosition, 1.0f), WVPLight);

	//output.clip = dot(output.positionScreen, clippingPlane);
	return output;
}

//   |
//   v
// Rasterizer
//   |
//   v

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 worldNormal = normalize(input.worldNormal);
	float3 worldTangent = normalize(input.worldTangent);
	float3 worldBinormal = normalize(cross(worldNormal, worldTangent));
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float3 normal = worldNormal;
	//if (normalMapWeight != 0.0f)
	//{
	//	float3x3 TBNW = { worldTangent, worldBinormal, worldNormal, };
	//	float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
	//	float3 normalSampled = (normalColor.xyz * 2.0f) - 1.0f;
	//	normal = mul(normalSampled, TBNW);
	//}

	float4 ambient = LightAmbient * MaterialAmbient;
	//if (aoWeight == 1.0f)
	//	ambient = aoMap.Sample(textureSampler, input.texCoord);

	float diffuseIntensity = saturate(dot(dirToLight, normal));
	float4 diffuse = diffuseIntensity * LightDiffuse * MaterialDiffuse;

	float3 halfAngle = normalize(dirToLight + dirToView);
	float specularBase = saturate(dot(halfAngle, normal));
	float specularIntensity = pow(specularBase, MaterialPower);
	float4 specular = specularIntensity * LightSpecular * MaterialSpecular;

	float2 UVRefraction = input.positionScreen.xy / input.positionScreen.w;
	UVRefraction = (UVRefraction + 1.0f) * 0.5f;
	float2 UVReflect = UVRefraction;
	UVRefraction.y = 1.0f - UVRefraction.y;
	
	
	//UVReflect.y = UV.x;
	//UVReflect.x = UV.y;
	
	float4 texColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float4 texColorRefraction = refractionMap.Sample(textureSampler, UVRefraction);
	float4 texColorReflection = reflectionMap.Sample(textureSampler, UVReflect);

	//texColor = lerp(texColor, float4(0.87f,0.88f,1.0f,1.0f), 0.5);
	//texColor = lerp(texColor, texColorRefraction, 0.5);
	//texColor = lerp(texColor, texColorReflection, 0.5);
	float specularFactor = specularMap.Sample(textureSampler, input.texCoord).r;

	float4 color = (ambient + diffuse) * texColor * brightness + specular * (specularMapWeight != 0.0f ? specularFactor : 1.0f);
	
	//color = lerp(color, texColorRefraction, 0.5);
	//color = lerp(color, texColorReflection, 0.5);
	
	float refractiveFactor = saturate(dot(dirToView, normal));
	float4 waterColor = lerp(texColorReflection, texColorRefraction, 0.5f);
	color = lerp(color, waterColor, 0.8f);

	//if (useShadow)
	//{
	//	float actualDepth = 1.0f - input.positionNDC.z / input.positionNDC.w;
	//	float2 shadowUV = input.positionNDC.xy / input.positionNDC.w;
	//	shadowUV = (shadowUV + 1.0f) * 0.5f;
	//	shadowUV.y = 1.0f - shadowUV.y;
	//	if (saturate(shadowUV.x) == shadowUV.x && saturate(shadowUV.y) == shadowUV.y)
	//	{
	//		float savedDepth = depthMap.Sample(textureSampler, shadowUV).r;
	//		if (savedDepth > actualDepth + depthBias)
	//		{
	//			color = ambient * texColor;
	//		}
	//	}
	//}
	return color;
}