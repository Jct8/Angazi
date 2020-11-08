#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoord;

out vec3 outWorldNormal;
out vec3 outWorldTangent;
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
	vec3 LightDirection;
	vec4 LightAmbient;
	vec4 LightDiffuse;
	vec4 LightSpecular;
	vec4 clippingPlane;
};

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D normalMap;

void main()
{
	vec4 outPos = vec4(aPos, 1.0f) * WVP;
	outWorldNormal = aNormal.xyz * mat3x3(World);
	outWorldTangent = aTangent.xyz * mat3x3(World);
	outDirToView = ViewPosition - (aPos.xyz * mat3x3(World));
	outTexCoord = aTexCoord * 10.0f;
	gl_ClipDistance = dot(vec4(aPos, 1.0f) * World, clippingPlane);

	gl_Position = outPos;
}

#shader PS
#version 450 core

in vec3 outWorldNormal;
in vec3 outWorldTangent;
in vec3 outDirToView;
in vec2 outTexCoord;

out vec4 FragColor;

layout(std140, binding = 0) uniform TransformBuffer
{
	mat4 World;
	mat4 WVP;
	vec3 ViewPosition;
	vec3 LightDirection;
	vec4 LightAmbient;
	vec4 LightDiffuse;
	vec4 LightSpecular;
	vec4 clippingPlane;
};

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D normalMap;

void main()
{
	vec3 normal = normalize(outWorldNormal);
	vec3 dirToLight = normalize(-LightDirection.xyz);
	vec3 dirToView = normalize(outDirToView);

	vec3 worldNormal = normalize(outWorldNormal);
	vec3 worldTangent = normalize(outWorldTangent);
	vec3 worldBinormal = normalize(cross(worldNormal,worldTangent));
	mat3x3 TBNW = { worldTangent, worldBinormal, worldNormal};
	vec4 normalColor = texture(normalMap, outTexCoord);
	vec3 normalSampled = (normalColor.xyz * 2.0f) - 1.0f;
	normal = TBNW * normalSampled;

	vec4 ambient = LightAmbient;
	vec4 diffuse = LightDiffuse * clamp(dot(normal,dirToLight),0.0,1.0);

	vec4 textureColor = texture(diffuseMap, outTexCoord);

	FragColor = (ambient + diffuse) * textureColor;
}