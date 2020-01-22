#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 position;
out vec3 color;

struct TransformBuffer
{
	uniform mat4 WVP;
	uniform mat4 World;
	uniform float3 ViewPosition;
}

struct LightBuffer 
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct MaterialBuffer 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular
	float power;
}; 

void main()
{
	vec3 worldPosition = float4(input.position, 1.0f), World);

	//ambient
	vec3 ambient = LightBuffer.ambient + MaterialBuffer.ambient;

	//diffuse
	vec3 dirToLight = -LightBuffer.direction;
	float diffuseIntensity = max(dot(dirToLight, worldNormal),0.0);
	vec4 diffuse = diffuseIntensity * LightBuffer.diffuse * MaterialBuffer.diffuse;

	//specular
	vec3 dirToView = normalize(ViewPosition -  worldPosition);
	vec3 halfAngle = normalize(dirToLight + dirToView);
	float  specularBase = max(dot(halfAngle, worldNormal), 0.0);
	float  specularIntensity = pow(specularBase, MaterialBuffer.power);
	vec4 specular = specularIntensity * LightBuffer.specular * MaterialBuffer.specular;

	vec4 color = ambient + diffuse + specular;

	gl_Position = WVP * vec4(aPos, 1.0);
	ourColor = color;
}

#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 ourColor;

void main()
{
	FragColor = vec4(ourColor, 1.0f);
}