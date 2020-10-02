#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoord;

out vec3 outDirection;
out vec3 outWorldNormal;
out vec3 outWorldTangent;
out vec3 outDirToLight;
out vec3 outDirToView;
out vec2 outTexCoord;

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
	float brightness;
	float environmentRatio;
	float reflectRefractRatio;
	float refractionIndex;
};

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform samplerCube cubeMap;

void main()
{
	vec3 localPosition = aPos;
	vec3 worldPosition = (vec4(localPosition, 1.0f) * World).xyz;
	vec3 worldNormal = aNormal.xyz * mat3x3(World);
	vec3 worldTangent = aTangent.xyz * mat3x3(World);

	outDirection = worldPosition - ViewPosition;
	outWorldNormal = worldNormal;
	outWorldTangent = worldTangent;
	outDirToLight = -LightDirection;
	outDirToView = normalize(ViewPosition - worldPosition);
	outTexCoord = aTexCoord;

	gl_Position = vec4(localPosition, 1.0f) * WVP;
}

#shader PS
#version 450 core

in vec3 outDirection;
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
	float brightness;
	float environmentRatio;
	float reflectRefractRatio;
	float refractionIndex;
};

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform samplerCube cubeMap;

void main()
{
	vec3 worldNormal = normalize(outWorldNormal);
	vec3 worldTangent = normalize(outWorldTangent);
	vec3 worldBinormal = normalize(cross(worldNormal,worldTangent));
	vec3 dirToLight = normalize(outDirToLight);
	vec3 dirToView = normalize(outDirToView);

	vec3 normal = worldNormal;
	// ambient
	vec4 ambient = LightAmbient * MaterialAmbient;

	// diffuse
	float diffuseIntensity = clamp(dot(dirToLight, normal),0.0,1.0);
	vec4 diffuse = diffuseIntensity * LightDiffuse* MaterialDiffuse;

	// specular
	vec3 halfAngle = normalize(dirToLight + dirToView);
	float specularBase = clamp(dot(halfAngle, normal), 0.0,1.0);
	float specularIntensity = pow(specularBase, MaterialPower);
	vec4 specular = specularIntensity * LightSpecular * MaterialSpecular;

	vec4 mainTexture = texture(diffuseMap, outTexCoord);
	vec4 color = (ambient + diffuse) * mainTexture * brightness + specular;

	// Environment Mapping
	vec3 reflectedVector = reflect(outDirection, normal);
	vec4 reflectedColor = texture(cubeMap, reflectedVector);
	
	vec3 refractedVector = refract(outDirection, normal,refractionIndex);
	vec4 refractedColor = texture(cubeMap, refractedVector);

	vec4 environmentColor = mix(reflectedColor,refractedColor,reflectRefractRatio);
	color = mix(color, environmentColor, environmentRatio);
	FragColor = color;
}