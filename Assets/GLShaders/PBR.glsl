#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in ivec4 aBlendIndices;
layout (location = 5) in vec4 aBlendWeights;

out vec3 outWorldNormal;
out vec3 outWorldTangent;
out vec3 outWorldPosition;
out vec3 outDirToLight;
out vec3 outDirToView;
out vec2 outTexCoord;
out vec4 outPositionNDC;

out gl_PerVertex
{
	vec4 gl_Position;
	float gl_ClipDistance;
};

layout(std140, binding = 0) uniform TransformBuffer
{
	mat4 World;
	mat4 WVP;
	vec3 ViewPosition;
};

layout(std140, binding = 1) uniform LightBuffer 
{
	vec3 LightDirection;
	vec4 LightAmbient;
	vec4 LightDiffuse;
	vec4 LightSpecular;
};

layout(std140, binding = 2) uniform MaterialBuffer 
{
	vec3 padding;
	float MaterialPower;
	vec4 MaterialAmbient;
	vec4 MaterialDiffuse;
	vec4 MaterialSpecular;
};

layout(std140, binding = 3) uniform SettingsBuffer 
{
	float bumpMapWeight;
	float normalMapWeight;
	float aoWeight;
	float brightness;
	bool useShadow;
	float depthBias;
	float isSkinnedMesh;
	float metallicWeight;
	float roughnessWeight;
	bool useIBL;
};

layout(std140, binding = 4) uniform ShadowBuffer 
{
	mat4x4 WVPLight;
};

layout(std140, binding = 5) uniform BoneTransformBuffer 
{
	mat4x4 boneTransforms[256];
};

layout(std140, binding = 6) uniform ClippingBuffer 
{
	vec4 clippingPlane;
};

mat4 Identity = mat4
	(1.0, 0.0, 0.0, 0.0,
	 0.0, 1.0, 0.0, 0.0,
	 0.0, 0.0, 1.0, 0.0,
	 0.0, 0.0, 0.0, 1.0); 

mat4 GetBoneTransform(ivec4 indices, vec4 weights)
{
	if (length(weights) <= 0.0f || isSkinnedMesh == 0.0f)
		return Identity;

	mat4 transform;
	transform = boneTransforms[indices[0]] * weights[0];
	transform += boneTransforms[indices[1]] * weights[1];
	transform += boneTransforms[indices[2]] * weights[2];
	transform += boneTransforms[indices[3]] * weights[3];
	return transform;
}

layout(binding = 0) uniform sampler2D albedoMap;
layout(binding = 1) uniform sampler2D displacementMap;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D aoMap;
layout(binding = 4) uniform sampler2D depthMap;
layout(binding = 5) uniform sampler2D metallicMap;
layout(binding = 6) uniform sampler2D roughnessMap;
layout(binding = 7) uniform samplerCube irradianceMap;
layout(binding = 8) uniform samplerCube prefilterMap;
layout(binding = 9) uniform sampler2D brdfLUT;

void main()
{
	float displacement =  texture(displacementMap, aTexCoord,0).x;
	vec3 localPosition = aPos + (aNormal.xyz * displacement * bumpMapWeight);

	mat4 toRoot = GetBoneTransform(aBlendIndices, aBlendWeights);
	mat4 toWorld = toRoot* World;
	mat4 toNDC = toRoot * WVP;
	mat4 toNDCLight = toRoot * WVPLight;

	vec3 worldPosition = (vec4(localPosition, 1.0f) * toWorld).xyz;
	vec3 worldNormal = aNormal.xyz * mat3x3(toWorld);
	vec3 worldTangent = aTangent.xyz * mat3x3(toWorld);

	vec4 outPos = vec4(localPosition, 1.0f) * toNDC;
	outWorldNormal = worldNormal;
	outWorldTangent = worldTangent;
	outWorldPosition = worldPosition;
	outDirToLight = -LightDirection;
	outDirToView = normalize(ViewPosition - worldPosition);
	outTexCoord = aTexCoord;

	if(useShadow)
		outPositionNDC = vec4(localPosition, 1.0f) * toNDCLight;

	gl_ClipDistance = dot(vec4(aPos, 1.0f)* World, clippingPlane);
	gl_Position = outPos;
}

#shader PS
#version 450 core

in vec3 outWorldNormal;
in vec3 outWorldTangent;
in vec3 outWorldPosition;
in vec3 outDirToLight;
in vec3 outDirToView;
in vec2 outTexCoord;
in vec4 outPositionNDC;

out vec4 FragColor;

layout(std140, binding = 0) uniform TransformBuffer
{
	mat4 World;
	mat4 WVP;
	vec3 ViewPosition;
};

layout(std140, binding = 1) uniform LightBuffer 
{
	vec3 LightDirection;
	vec4 LightAmbient;
	vec4 LightDiffuse;
	vec4 LightSpecular;
};

layout(std140, binding = 2) uniform MaterialBuffer 
{
	vec3 padding;
	float MaterialPower;
	vec4 MaterialAmbient;
	vec4 MaterialDiffuse;
	vec4 MaterialSpecular;
};
layout(std140, binding = 3) uniform SettingsBuffer 
{
	float bumpMapWeight;
	float normalMapWeight;
	float aoWeight;
	float brightness;
	bool useShadow;
	float depthBias;
	float isSkinnedMesh;
	float metallicWeight;
	float roughnessWeight;
	bool useIBL;
};

layout(std140, binding = 4) uniform ShadowBuffer 
{
	mat4x4 WVPLight;
};

layout(std140, binding = 5) uniform BoneTransformBuffer 
{
	mat4x4 boneTransforms[256];
};

layout(std140, binding = 6) uniform ClippingBuffer 
{
	vec4 clippingPlane;
};

layout(binding = 0) uniform sampler2D albedoMap;
layout(binding = 1) uniform sampler2D displacementMap;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D aoMap;
layout(binding = 4) uniform sampler2D depthMap;
layout(binding = 5) uniform sampler2D metallicMap;
layout(binding = 6) uniform sampler2D roughnessMap;
layout(binding = 7) uniform samplerCube irradianceMap;
layout(binding = 8) uniform samplerCube prefilterMap;
layout(binding = 9) uniform sampler2D brdfLUT;

const float PI = 3.14159265359;
const float EPSILON = 1e-6f;
const vec3 fDielectric = vec3(0.04f, 0.04f, 0.04f);

float DistributionGGX(vec3 normal, vec3 halfVector, float roughness)
{
	float alpha = roughness * roughness;
	float alphaSquared = alpha * alpha;
	float nDotH = max(dot(normal, halfVector), 0.0f);
	float denominator = (nDotH * nDotH) * (alphaSquared - 1.0f) + 1.0f;
	return alphaSquared / max( denominator * denominator, EPSILON);
	//https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/ //
}

float GeometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness)
{
	float r = roughness + 1.0f;
	float kDirect = (r * r) / 8.0f;
	float nDotV = max(dot(normal, viewDir), 0.0f);
	float nDotL = max(dot(normal, lightDir), 0.0f);

	float ggx1 = nDotV / (nDotV * (1.0f - kDirect) + kDirect);
	float ggx2 = nDotL / (nDotL * (1.0f - kDirect) + kDirect);
	return ggx1 * ggx2;
}

vec3 FresnalSchlick(float cosTheta, vec3 f0)
{
	cosTheta = min(cosTheta, 1.0f);
	return max(f0 + (1.0f - f0) * pow(1.0f - cosTheta, 5.0f), 0.0f);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0f - cosTheta, 5.0f);
}

void main()
{
	vec3 worldNormal = normalize(outWorldNormal);
	vec3 worldTangent = normalize(outWorldTangent);
	vec3 worldBinormal = normalize(cross(worldNormal,worldTangent));
	vec3 dirToLight = normalize(outDirToLight);
	vec3 dirToView = normalize(outDirToView);

	vec3 albedoColor = texture(albedoMap, outTexCoord).rgb;
	float metallic = texture(metallicMap, outTexCoord).r;
	if (metallicWeight >= 0.0f)
		metallic = metallicWeight;
	float roughness = texture(roughnessMap, outTexCoord).r;
	if (roughnessWeight >= 0.0f)
		roughness = roughnessWeight;

	vec3 normal = worldNormal;
	if (normalMapWeight != 0.0f)
	{
		if (!(outWorldTangent.x == 0.0f && outWorldTangent.y == 0.0f && outWorldTangent.z == 0.0f))
		{
			mat3x3 TBNW = { worldTangent, worldBinormal, worldNormal};
			vec4 normalColor = texture(normalMap, outTexCoord);
			vec3 normalSampled = (normalColor.xyz * 2.0f) - 1.0f;
			normal = TBNW * normalSampled;
		}
		else
		{
			vec3 tangentNormal = texture(normalMap, outTexCoord).xyz * 2.0 - 1.0;
			vec3 Q1 = dFdx(outWorldPosition);
			vec3 Q2 = dFdy(outWorldPosition);
			vec2 st1 = dFdx(outTexCoord);
			vec2 st2 = dFdy(outTexCoord);
			vec3 T = normalize(Q1 * st2.x - Q2 * st1.x);
			vec3 B = -normalize(cross(worldNormal, T));
			mat3  TBN = mat3( T, B, worldNormal );
			normal = normalize(TBN * tangentNormal);
		}
	}

	////// PBR Lighting //////
	float ambientOcclusion = 1.0f;
	if (aoWeight == 1.0f)
		ambientOcclusion = texture(aoMap, outTexCoord).r;
	vec3 diffuseColor = mix(albedoColor, vec3(0.0f), metallic);
	vec3 f0 = mix(fDielectric, albedoColor, metallic);
	vec3 V = dirToView;
	vec3 L = dirToLight;
	vec3 R = reflect(V, normal);
	vec3 halfVector = normalize(V + L);
	float nDotL = max(dot(normal, L), 0.0f);
	float nDotV = max(dot(normal, V), 0.0f);

	// Ambient
	vec4 ambient = vec4(albedoColor, 1.0f)  *LightAmbient * MaterialAmbient;

	// Specular
	float D = DistributionGGX(normal, halfVector, roughness); 
	float G = GeometrySmith(normal, V, L, roughness);
	vec3 F = FresnalSchlick(nDotV, f0);
	vec3 specularBRDF = (D * G * F) / max(4.0f * nDotV * nDotL, EPSILON) * LightSpecular.rgb * MaterialSpecular.rgb; // Cook-Torrance specular

	// Diffuse
	vec3 kDiffuse = (vec3(1.0f) - F) * (1.0f - metallic);
	vec3 diffuseBRDF = kDiffuse * albedoColor * LightDiffuse.rgb * MaterialDiffuse.rgb;

	vec3 directLighting = (diffuseBRDF + specularBRDF) * brightness * nDotL;

	////// IBL //////
	if (useIBL)
	{
		// Diffuse
		vec3 kS = FresnelSchlickRoughness(nDotV, f0, roughness);
		vec3 kD = (vec3(1.0f) - kS) * (1.0f - metallic);
		vec3 diffuse = texture(irradianceMap, normal).rgb * albedoColor * kD;

		//Specular
		const float maxReflectionLOD = 4.0;
		R.y = -R.y;
		vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * maxReflectionLOD).rgb;
		vec2 brdf = texture(brdfLUT, vec2(max(dot(normal, V), 0.0), roughness)).rg;
		vec3 specular = prefilteredColor * (kS * brdf.x + brdf.y);

		ambient = vec4(diffuse + specular, 1.0f);
	}
	vec3 color = (ambient.rgb + directLighting) * ambientOcclusion;

	// Shadowing
	if (useShadow)
	{
		float actualDepth = 1.0f - outPositionNDC.z / outPositionNDC.w;
		vec2 shadowUV = outPositionNDC.xy / outPositionNDC.w;
		shadowUV = (shadowUV + 1.0f) *0.5f;
		//shadowUV.y = 1.0f - shadowUV.y;
		if (clamp(shadowUV.x,0.0f,1.0f) == shadowUV.x && clamp(shadowUV.y,0.0f,1.0f) == shadowUV.y)
		{
			float savedDepth = texture(depthMap, shadowUV).r;
			if (savedDepth > actualDepth + depthBias)
			{
				color = ambient.rgb * albedoColor;
			}
		}
	}
	FragColor = vec4(color,1.0f);
}