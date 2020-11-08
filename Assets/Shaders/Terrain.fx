//Description: Terrain Shader for Angazi

cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix WVP;
	float3 ViewPosition;
	float3 LightDirection;
	float4 LightAmbient;
	float4 LightDiffuse;
	float4 LightSpecular;
	float4 clippingPlane;
}

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 worldNormal : NORMAL;
	float3 worldTangent :TEXCOORD0;
	float3 dirToView : TEXCOORD1;
	float2 texCoord	: TEXCOORD2;
	float clip : SV_ClipDistance0;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(float4(input.position, 1.0f), WVP);
	output.worldNormal = mul(input.normal,(float3x3) World);
	output.worldTangent = mul(input.tangent, (float3x3) World);
	output.dirToView = ViewPosition - mul(input.position.xyz, (float3x3) World);
	output.texCoord = input.texCoord * 10.0f;
	output.clip = dot(mul(float4(input.position, 1.0f), World), clippingPlane);
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 normal = normalize(input.worldNormal);
	float3 dirToLight = normalize(-LightDirection.xyz);
	float3 dirToView = normalize(input.dirToView);

	float3 worldNormal = normal;
	float3 worldTangent = normalize(input.worldTangent);
	float3 worldBinormal = normalize(cross(worldNormal, worldTangent));
	float3x3 TBNW = { worldTangent, worldBinormal, worldNormal };
	float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
	float3 normalSampled = (normalColor.xyz * 2.0f) - 1.0f;
	normal = mul(normalSampled, TBNW);

	float4 ambient = LightAmbient;
	float4 diffuse = LightDiffuse * saturate(dot(normal,dirToLight));

	float4 textureColor = diffuseMap.Sample(textureSampler, input.texCoord);

	return (ambient + diffuse) * textureColor;
}