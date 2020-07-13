// Description: PBR shader for Angazi Engine
// PBR based on https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf

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
	float4 MaterialAmbient;
	float4 MaterialDiffuse;
	float4 MaterialSpecular;
	float MaterialPower;
}

cbuffer SettingsBuffer : register(b3)
{
	float bumpMapWeight : packoffset(c0.x);
	float normalMapWeight : packoffset(c0.y);
	float aoWeight : packoffset(c0.z);
	float brightness : packoffset(c0.w);
	bool useShadow : packoffset(c1.x);
	float depthBias : packoffset(c1.y);
	float isSkinnedMesh : packoffset(c1.z);
	float metallicWeight : packoffset(c1.w);
	float roughnessWeight : packoffset(c2.x);
	bool useIBL : packoffset(c2.y);
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

Texture2D albedoMap : register(t0);
Texture2D displacementMap : register(t1);
Texture2D normalMap : register(t2);
Texture2D aoMap : register(t3);
Texture2D depthMap : register(t4);
Texture2D metallicMap : register(t5);
Texture2D roughnessMap : register(t6);
TextureCube irradianceMap : register(t7);

SamplerState textureSampler : register(s0);

static matrix Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};
static const float PI = 3.14159265359;
static const float EPSILON = 1e-6f;
const float3 fDielectric = float3(0.04f, 0.04f, 0.04f);

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

float DistributionGGX(float3 normal, float3 halfVector, float roughness)
{
	float alpha = roughness * roughness;
	float alphaSquared = alpha * alpha;
	float nDotH = max(dot(normal, halfVector), 0.0f);
	float denominator = (nDotH * nDotH) * (alphaSquared - 1.0f) + 1.0f;
	return alphaSquared / max(denominator * denominator, EPSILON);
	//https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/ //
}

float GeometrySmith(float3 normal, float3 viewDir, float3 lightDir, float roughness)
{
	float r = roughness + 1.0f;
	float kDirect = (r * r) / 8.0f;
	float nDotV = max(dot(normal, viewDir), 0.0f);
	float nDotL = max(dot(normal, lightDir), 0.0f);

	float ggx1 = nDotV / (nDotV * (1.0f - kDirect) + kDirect);
	float ggx2 = nDotL / (nDotL * (1.0f - kDirect) + kDirect);
	return ggx1 * ggx2;
}

float3 FresnalSchlick(float cosTheta, float3 f0)
{
	cosTheta = min(cosTheta, 1.0f);
	return max(f0 + (1.0f - f0) * pow(1.0f - cosTheta, 5.0f), 0.0f);
}

float3 FresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
	return F0 + (max(float3(1.0f - roughness, 1.0f - roughness, 1.0f - roughness), F0) - F0) * pow(1.0f - cosTheta, 5.0f);
}

struct VS_INPUT
{
	float3 position : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
	int4 blendIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHT;
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
	output.worldPosition = worldPosition;
	output.worldNormal = worldNormal;
	output.worldTangent = worldTangent;
	output.dirToLight = -LightDirection;
	output.dirToView = normalize(ViewPosition - worldPosition);
	output.texCoord = input.texCoord;

	if (useShadow)
		output.positionNDC = mul(float4(localPosition, 1.0f), toNDCLight);

	output.clip = dot(mul(float4(input.position, 1.0f), World), clippingPlane);
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 worldNormal = normalize(input.worldNormal);
	float3 worldTangent = normalize(input.worldTangent);
	float3 worldBinormal = normalize(cross(worldNormal, worldTangent));
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float3 albedoColor = albedoMap.Sample(textureSampler, input.texCoord).rgb;
	float metallic = metallicMap.Sample(textureSampler, input.texCoord).r;
	if (metallicWeight >= 0.0f)
		metallic = metallicWeight;
	float roughness = roughnessMap.Sample(textureSampler, input.texCoord).r;
	if (roughnessWeight >= 0.0f)
		roughness = roughnessWeight;

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

	////// PBR Lighting //////
	float ambientOcclusion = 1.0f;
	if (aoWeight == 1.0f)
		ambientOcclusion = aoMap.Sample(textureSampler, input.texCoord).r;
	float3 f0 = lerp(fDielectric, albedoColor, metallic);
	float3 V = dirToView;
	float3 L = dirToLight;
	float3 halfVector = normalize(V + L);
	float nDotL = max(dot(normal, L), 0.0f);
	float nDotV = max(dot(normal, V), 0.0f);

	// Ambient
	float4 ambient = float4(albedoColor, 1.0f) * LightAmbient * MaterialAmbient;

	// Specular
	float D = DistributionGGX(normal, halfVector, roughness);
	float G = GeometrySmith(normal, V, L, roughness);
	float3 F = FresnalSchlick(nDotV, f0);
	float3 specularBRDF = (D * G * F) / max(4.0f * nDotV * nDotL, EPSILON) * LightSpecular.rgb * MaterialSpecular.rgb; // Cook-Torrance specular
	
	// Diffuse
	float3 kDiffuse = (float3(1.0f, 1.0f, 1.0f) - F) * (1.0f - metallic);
	float3 diffuseBRDF = kDiffuse * albedoColor * LightDiffuse.rgb * MaterialDiffuse.rgb;

	float3 directLighting = (diffuseBRDF + specularBRDF) * brightness * nDotL;

	////// IBL //////
	if (useIBL)
	{
		// Diffuse
		float3 kS = FresnalSchlick(nDotV, f0);
		float3 kD = (float3(1.0f, 1.0f, 1.0f) - kS) * (1.0f - metallic);
		float3 diffuse = irradianceMap.Sample(textureSampler, normal).rgb * albedoColor * kD;
		ambient = float4(diffuse,1.0f);

		//Specular

	}

	float3 color = (ambient.rgb + directLighting) * ambientOcclusion;
	// Shadowing
	if (useShadow)
	{
		float actualDepth = 1.0f - input.positionNDC.z / input.positionNDC.w;
		float2 shadowUV = input.positionNDC.xy / input.positionNDC.w;
		shadowUV = (shadowUV + 1.0f) * 0.5f;
		shadowUV.y = 1.0f - shadowUV.y;
		if (saturate(shadowUV.x) == shadowUV.x && saturate(shadowUV.y) == shadowUV.y)
		{
			float savedDepth = depthMap.Sample(textureSampler, shadowUV).r;
			if (savedDepth > actualDepth + depthBias)
			{
				color = ambient * float4(albedoColor, 1.0);
			}
		}
	}

	return float4(color, 1.0f);
}