#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoord;

out vec3 outPos;
out vec3 outWorldNormal;
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

layout(binding = 0) uniform sampler2D displacementMap;
layout(binding = 1) uniform sampler2D diffuseMap;
layout(binding = 2) uniform sampler2D specularMap;

void main()
{
	float displacement =  texture(displacementMap, aTexCoord,0).x;
	vec3 localPosition = aPos; //+ (aNormal.xyz * displacement * LightAmbient.a);
	vec3 worldPosition = (vec4(localPosition, 1.0f)* World).xyz;
	vec3 worldNormal = aNormal.xyz * mat3x3(World);

	vec4 outPos = vec4(localPosition, 1.0f) * WVP;
	outWorldNormal = worldNormal;
	outDirToLight = -LightDirection;
	outDirToView = normalize(ViewPosition - worldPosition);
	outTexCoord = aTexCoord;
	gl_Position = outPos;
}

#shader PS
#version 450 core

in vec3 outPos;
in vec3 outWorldNormal;
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

layout(binding = 0) uniform sampler2D displacementMap;
layout(binding = 1) uniform sampler2D diffuseMap;
layout(binding = 2) uniform sampler2D specularMap;

void main()
{
	vec3 worldNormal = normalize(outWorldNormal);
	vec3 dirToLight = normalize(outDirToLight - outPos);
	vec3 dirToView = normalize(outDirToView);

	//ambient
	vec4 ambient = LightAmbient * MaterialAmbient;

	//diffuse
	float diffuseIntensity = clamp(dot(dirToLight, worldNormal),0.0,1.0);
	vec4 diffuse = diffuseIntensity * LightDiffuse* MaterialDiffuse;

	//specular
	vec3 halfAngle = normalize(dirToLight + dirToView);
	float specularBase = clamp(dot(halfAngle, worldNormal), 0.0,1.0);
	float specularIntensity = pow(specularBase, MaterialPower);
	vec4 specular = specularIntensity * LightSpecular * MaterialSpecular;

	vec4 mainTexture = texture(diffuseMap, outTexCoord);
	float specularFactor = texture(specularMap, outTexCoord).r;

	vec4 color = (ambient + diffuse) * mainTexture + specular * specularFactor;
	FragColor = color;
}