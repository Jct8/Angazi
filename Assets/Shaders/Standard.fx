//Description: Standard shader for Angazi Engine

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
	float  MaterialPower; //shininess
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
	float isSkinnedMesh : packoffset(c1.w);
}

cbuffer ShadowBuffer : register(b4)
{
	matrix WVPLight;
}

cbuffer BoneTransformBuffer : register(b5)
{
	matrix boneTransforms[256];
}

cbuffer ClippingBuffer : register(b6)
{
	float4 clippingPlane;
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D aoMap : register(t4);
Texture2D depthMap : register(t5);

SamplerState textureSampler : register(s0);

static matrix Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

matrix GetBoneTransform(int4 indices, float4 weights)
{
	if (length(weights) <= 0.0f || isSkinnedMesh == 0.0f)
		return Identity;

	matrix transform;
	transform = boneTransforms[indices[0]] * weights[0];
	transform += boneTransforms[indices[1]] * weights[1];
	transform += boneTransforms[indices[2]] * weights[2];
	transform += boneTransforms[indices[3]] * weights[3];
	return transform;
}

struct VS_INPUT
{
	float3 position : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float2 texCoord	: TEXCOORD;
	int4 blendIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHT;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 worldNormal : NORMAL;
	float3 worldTangent :TEXCOORD0;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
	float2 texCoord	: TEXCOORD3;
	float4 positionNDC : TEXCOORD4;
	float3 worldPosition : TEXCOORD5;
	float clip : SV_ClipDistance0;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float3 localPosition = input.position + (input.normal.xyz * displacement * bumpMapWeight);

	matrix toRoot = GetBoneTransform(input.blendIndices, input.blendWeights);
	matrix toWorld = mul(toRoot, World);
	matrix toNDC = mul(toRoot, WVP);
	matrix toNDCLight = mul(toRoot, WVPLight);
	
	float3 worldPosition = mul(float4(localPosition, 1.0f), toWorld).xyz;
	float3 worldNormal = mul(input.normal.xyz, (float3x3) toWorld);
	float3 worldTangent = mul(input.tangent.xyz, (float3x3) toWorld);

	output.position = mul(float4(localPosition, 1.0f), toNDC);
	output.worldNormal = worldNormal;
	output.worldTangent = worldTangent;
	output.worldPosition = worldPosition;
	output.dirToLight = -LightDirection;
	output.dirToView = normalize(ViewPosition - worldPosition);
	output.texCoord = input.texCoord;

	if(useShadow)
		output.positionNDC = mul(float4(localPosition, 1.0f), toNDCLight);

	output.clip = dot(mul(float4(input.position, 1.0f), World), clippingPlane);

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
	float3 worldBinormal = normalize(cross(worldNormal,worldTangent));
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float3 normal = worldNormal;
	if (normalMapWeight != 0.0f)
	{
		if (!all(worldTangent))
		{
			float3x3 TBNW = { worldTangent, worldBinormal, worldNormal };
			float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
			float3 normalSampled = (normalColor.xyz * 2.0f) - 1.0f;
			normal = mul(normalSampled, TBNW);
		}
		else
		{
			float3 tangentNormal = normalMap.Sample(textureSampler, input.texCoord).xyz * 2.0 - 1.0;
			float3 Q1 = ddx(input.worldPosition);
			float3 Q2 = ddy(input.worldPosition);
			float2 st1 = ddx(input.texCoord);
			float2 st2 = ddy(input.texCoord);
			float3 T = normalize(Q1 * st2.y - Q2 * st1.y);
			float3 B = -normalize(cross(worldNormal, T));
			float3x3 TBN = { T, B, worldNormal };
			normal = normalize(mul(tangentNormal, TBN));
		}
	}

	float ambientOcclusion = 1.0f;
	if(aoWeight == 1.0f)
		ambientOcclusion = aoMap.Sample(textureSampler, input.texCoord).r;
	float4 ambient = LightAmbient * MaterialAmbient;

	float diffuseIntensity = saturate(dot(dirToLight, normal));
	float4 diffuse = diffuseIntensity * LightDiffuse * MaterialDiffuse;

	float3 halfAngle = normalize(dirToLight + dirToView);
	float  specularBase = saturate(dot(halfAngle, normal));
	float  specularIntensity = pow(specularBase, MaterialPower);
	float4 specular = specularIntensity * LightSpecular * MaterialSpecular;

	float4 texColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float specularFactor = specularMap.Sample(textureSampler, input.texCoord).r;

	float4 color = (ambient + diffuse) * texColor * brightness * ambientOcclusion + specular * (specularMapWeight != 0.0f ? specularFactor : 1.0f);

	if (useShadow)
	{
		float actualDepth = 1.0f - input.positionNDC.z / input.positionNDC.w;
		float2 shadowUV = input.positionNDC.xy / input.positionNDC.w;
		shadowUV = (shadowUV + 1.0f) *0.5f;
		shadowUV.y = 1.0f - shadowUV.y;
		if (saturate(shadowUV.x) == shadowUV.x && saturate(shadowUV.y) == shadowUV.y)
		{
			float savedDepth = depthMap.Sample(textureSampler, shadowUV).r;
			if (savedDepth > actualDepth + depthBias)
			{
				color = ambient * texColor;
			}
		}
	}
	return color;
}