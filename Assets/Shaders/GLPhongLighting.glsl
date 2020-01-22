#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 position;
out vec3 color;

struct TransformBuffer
{
	mat4 WVP;
	mat4 World;
	vec3 ViewPosition;
};

struct LightBuffer 
{
	vec3 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct MaterialBuffer 
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular
	float power;
}; 

uniform MaterialBuffer material;
uniform LightBuffer light;
uniform TransformBuffer transform;

void main()
{
	vec3 worldPosition = vec4(aPos, 1.0f), transform.World);

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
in vec4 ourColor;

void main()
{
	FragColor = ourColor;
}