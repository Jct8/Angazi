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
out vec3 outDirToLight;
out vec3 outDirToView;
out vec2 outTexCoord;
out vec4 outPositionNDC;

out gl_PerVertex
{
	vec4 gl_Position;
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
	vec4 MaterialAmbient;
	vec4 MaterialDiffuse;
	vec4 MaterialSpecular;
	float MaterialPower;
};

layout(std140, binding = 3) uniform SettingsBuffer 
{
	float specularMapWeight;
	float bumpMapWeight;
	float normalMapWeight;
	float aoWeight;
	float brightness;
	bool useShadow;
	float depthBias;
	float isSkinnedMesh;
};

layout(std140, binding = 4) uniform ShadowBuffer 
{
	mat4x4 WVPLight;
};

layout(std140, binding = 5) uniform BoneTransformBuffer 
{
	mat4x4 boneTransforms[256];
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

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D specularMap;
layout(binding = 2) uniform sampler2D displacementMap;
layout(binding = 3) uniform sampler2D normalMap;
layout(binding = 4) uniform sampler2D aoMap;
layout(binding = 5) uniform sampler2D depthMap;

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
	outDirToLight = -LightDirection;
	outDirToView = normalize(ViewPosition - worldPosition);
	outTexCoord = aTexCoord;

	if(useShadow)
		outPositionNDC = vec4(localPosition, 1.0f) * toNDCLight;

	gl_Position = outPos;
}

#shader PS
#version 450 core

in vec3 outWorldNormal;
in vec3 outWorldTangent;
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
	vec4 MaterialAmbient;
	vec4 MaterialDiffuse;
	vec4 MaterialSpecular;
	float MaterialPower;
};

layout(std140, binding = 3) uniform SettingsBuffer 
{
	float specularMapWeight;
	float bumpMapWeight;
	float normalMapWeight;
	float aoWeight;
	float brightness;
	bool useShadow;
	float depthBias;
	bool isSkinnedMesh;
};

layout(std140, binding = 4) uniform ShadowBuffer 
{
	mat4x4 WVPLight;
};

layout(std140, binding = 5) uniform BoneTransformBuffer 
{
	mat4x4 boneTransforms[256];
};

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D specularMap;
layout(binding = 2) uniform sampler2D displacementMap;
layout(binding = 3) uniform sampler2D normalMap;
layout(binding = 4) uniform sampler2D aoMap;
layout(binding = 5) uniform sampler2D depthMap;

void main()
{
	vec3 worldNormal = normalize(outWorldNormal);
	vec3 worldTangent = normalize(outWorldTangent);
	vec3 worldBinormal = normalize(cross(worldTangent,worldNormal));
	vec3 dirToLight = normalize(outDirToLight);
	vec3 dirToView = normalize(outDirToView);

	vec3 normal = worldNormal;
	if (normalMapWeight != 0.0f)
	{
		mat3x3 TBNW = { worldTangent, worldBinormal, worldNormal};
		vec4 normalColor = texture(normalMap, outTexCoord);
		vec3 normalSampled = (normalColor.xyz * 2.0f) - 1.0f;
		normal = normalSampled * TBNW;
	}

	// ambient
	vec4 ambient = LightAmbient * MaterialAmbient;
	if(aoWeight == 1.0f)
		ambient = texture(aoMap, outTexCoord);

	// diffuse
	float diffuseIntensity = clamp(dot(dirToLight, normal),0.0,1.0);
	vec4 diffuse = diffuseIntensity * LightDiffuse* MaterialDiffuse;

	// specular
	vec3 halfAngle = normalize(dirToLight + dirToView);
	float specularBase = clamp(dot(halfAngle, normal), 0.0,1.0);
	float specularIntensity = pow(specularBase, MaterialPower);
	vec4 specular = specularIntensity * LightSpecular * MaterialSpecular;

	vec4 mainTexture = texture(diffuseMap, outTexCoord);
	float specularFactor = texture(specularMap, outTexCoord).r;

	vec4 color = (ambient + diffuse) * mainTexture * brightness + specular * (specularMapWeight != 0.0f ? specularFactor : 1.0f);
	if (useShadow)
	{
		float actualDepth = 1.0f - outPositionNDC.z / outPositionNDC.w;
		vec2 shadowUV = outPositionNDC.xy / outPositionNDC.w;
		shadowUV = (shadowUV + 1.0f) *0.5f;
		//shadowUV.y = 1.0f - shadowUV.y;
		if (clamp(shadowUV.x,0.0,1.0) == shadowUV.x && clamp(shadowUV.y,0.0,1.0) == shadowUV.y)
		{
			float savedDepth = texture(depthMap, shadowUV).r;
			if (savedDepth > actualDepth + depthBias)
			{
				color = ambient * mainTexture;
			}
		}
	}
	FragColor = color;
}