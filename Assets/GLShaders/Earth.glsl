#shader VSEarth
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoord;

out vec4 outPos;
out vec3 outWorldNormal;
out vec3 outWorldTangent;
out vec3 outDirToLight;
out vec3 outDirToView;
out vec2 outTexCoord;

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
	vec3 padding;
	float MaterialPower;
	vec4 MaterialAmbient;
	vec4 MaterialDiffuse;
	vec4 MaterialSpecular;
};

layout(std140, binding = 3) uniform SettingsBuffer 
{
	float specularMapWeight;
	float bumpMapWeight;
	float normalMapWeight;
};

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D specularMap;
layout(binding = 2) uniform sampler2D displacementMap;
layout(binding = 3) uniform sampler2D normalMap;
layout(binding = 4) uniform sampler2D nightMap;
layout(binding = 5) uniform sampler2D cloudMap;

void main()
{
	float displacement =  texture(displacementMap, aTexCoord,0).x;
	vec3 localPosition = aPos + (aNormal.xyz * displacement * bumpMapWeight);
	vec3 worldPosition = (vec4(localPosition, 1.0f)* World).xyz;
	vec3 worldNormal = aNormal.xyz * mat3x3(World);
	vec3 worldTangent = aTangent.xyz * mat3x3(World);

	outPos = vec4(localPosition, 1.0f) * WVP;
	outWorldNormal = worldNormal;
	outWorldTangent = worldTangent;
	outDirToLight = -LightDirection;
	outDirToView = normalize(ViewPosition - worldPosition);
	outTexCoord = aTexCoord;
	gl_Position = outPos;
}

#shader PSEarth
#version 450 core

in vec4 outPos;
in vec3 outWorldNormal;
in vec3 outWorldTangent;
in vec3 outDirToLight;
in vec3 outDirToView;
in vec2 outTexCoord;

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
	float specularMapWeight;
	float bumpMapWeights;
	float normalMapWeight;
};

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D specularMap;
layout(binding = 2) uniform sampler2D displacementMap;
layout(binding = 3) uniform sampler2D normalMap;
layout(binding = 4) uniform sampler2D nightMap;
layout(binding = 5) uniform sampler2D cloudMap;

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
		mat3x3 TBNW = { worldTangent, worldBinormal, worldNormal };
		vec4 normalColor = texture(normalMap, outTexCoord);
		vec3 normalSampled = (normalColor.xyz * 2.0f) - 1.0f;
		normal = normalSampled * TBNW;
	}

	//ambient
	vec4 ambient = LightAmbient * MaterialAmbient;

	//diffuse
	float diffuseIntensity = clamp(dot(dirToLight, normal),0.0,1.0);
	vec4 diffuse = diffuseIntensity * LightDiffuse* MaterialDiffuse;

	//specular
	vec3 halfAngle = normalize(dirToLight + dirToView);
	float specularBase = clamp(dot(halfAngle, normal), 0.0,1.0);
	float specularIntensity = pow(specularBase, MaterialPower);
	vec4 specular = specularIntensity * LightSpecular * MaterialSpecular;

	vec4 mainTexture = texture(diffuseMap, outTexCoord);
	float specularFactor = texture(specularMap, outTexCoord).r;

	float alpha = clamp(-dot(dirToLight, normal), 0.0,1.0);
	vec4 nightTexColor = texture(nightMap, outTexCoord);
	vec4 texColorNight = mix(mainTexture, nightTexColor, alpha);

	vec4 color = (ambient + diffuse) * mainTexture  + nightTexColor * alpha + specular * (specularMapWeight != 0.0f ? specularFactor : 1.0f);
	FragColor = color;
}

#shader VSCloud
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoord;

out vec3 outWorldNormal;
out vec3 outDirToLight;
out vec2 outTexCoord;

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
};

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D specularMap;
layout(binding = 2) uniform sampler2D displacementMap;
layout(binding = 3) uniform sampler2D normalMap;
layout(binding = 4) uniform sampler2D nightMap;
layout(binding = 5) uniform sampler2D cloudMap;
void main()
{
	vec3 localPosition = aPos + (aNormal.xyz * (0.1f + bumpMapWeight * 0.5f));
	vec3 worldNormal = aNormal.xyz * mat3x3(World);

	vec4 outPos = vec4(localPosition, 1.0f) * WVP;
	outWorldNormal = worldNormal;
	outDirToLight = -LightDirection;
	outTexCoord = aTexCoord;
	gl_Position = outPos;
}

#shader PSCloud
#version 450 core
in vec3 outWorldNormal;
in vec3 outDirToLight;
in vec2 outTexCoord;

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
};

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D specularMap;
layout(binding = 2) uniform sampler2D displacementMap;
layout(binding = 3) uniform sampler2D normalMap;
layout(binding = 4) uniform sampler2D nightMap;
layout(binding = 5) uniform sampler2D cloudMap;

void main()
{
	vec3 normal = normalize(outWorldNormal);
	vec3 dirToLight = normalize(outDirToLight);

	vec4 ambient = LightAmbient * MaterialAmbient;

	float diffuseIntensity = clamp(dot(dirToLight, normal),0.0,1.0);
	vec4 diffuse = diffuseIntensity * LightDiffuse * MaterialDiffuse;

	FragColor =  texture(cloudMap, outTexCoord) * diffuse;
}