//Description: Environment Map shader for Angazi Engine

cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix WVP; 
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
	float3 padding;
	float  MaterialPower; //shininess
	float4 MaterialAmbient;
	float4 MaterialDiffuse;
	float4 MaterialSpecular;
}

cbuffer SettingsBuffer : register(b3)
{
	float brightness : packoffset(c0.x);
	float environmentRatio : packoffset(c0.y);
	float reflectRefractRatio : packoffset(c0.z);
	float refractionIndex : packoffset(c0.w);
}

Texture2D diffuseMap : register(t0);
TextureCube cubeMap : register(t1);

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
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
	float2 texCoord	: TEXCOORD3;
	float3 dirToCamera : TEXCOORD4;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	float3 localPosition = input.position;
	float3 worldPosition = mul(float4(localPosition, 1.0f), World).xyz;
	float3 worldNormal = mul(input.normal.xyz, (float3x3) World);
	float3 worldTangent = mul(input.tangent.xyz, (float3x3) World);

	output.position = mul(float4(localPosition, 1.0f), WVP);
	output.worldNormal = worldNormal;
	output.worldTangent = worldTangent;
	output.dirToLight = -LightDirection;
	output.dirToView = normalize(ViewPosition - worldPosition);
	output.texCoord = input.texCoord;
	output.dirToCamera = worldPosition - ViewPosition;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 worldNormal = normalize(input.worldNormal);
	float3 worldTangent = normalize(input.worldTangent);
	float3 worldBinormal = normalize(cross(worldNormal,worldTangent));
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	// Lighting
	float3 normal = worldNormal;

	float4 ambient = LightAmbient * MaterialAmbient;

	float diffuseIntensity = saturate(dot(dirToLight, normal));
	float4 diffuse = diffuseIntensity * LightDiffuse * MaterialDiffuse;

	float3 halfAngle = normalize(dirToLight + dirToView);
	float  specularBase = saturate(dot(halfAngle, normal));
	float  specularIntensity = pow(specularBase, MaterialPower);
	float4 specular = specularIntensity * LightSpecular * MaterialSpecular;

	float4 texColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float4 color = (ambient + diffuse) * texColor * brightness + specular;

	// Environment Mapping
	float3 reflectedVector = reflect(input.dirToCamera, normal);
	float4 reflectedColor = cubeMap.Sample(textureSampler, reflectedVector);
	
	float3 refractedVector = refract(input.dirToCamera, normal, refractionIndex);
	float4 refractedColor = cubeMap.Sample(textureSampler, refractedVector);
	
	float4 environmentColor = lerp(reflectedColor,refractedColor,reflectRefractRatio);
	color = lerp(color, environmentColor, environmentRatio);
	return color;
}