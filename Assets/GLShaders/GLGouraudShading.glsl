#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;

out vec4 outColor;

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
	vec4 specular;
	float power;
}; 

uniform MaterialBuffer material;
uniform LightBuffer light;
uniform TransformBuffer transform;

void main()
{
	vec3 worldPosition = (vec4(aPos, 1.0f)*transform.World).xyz;
	vec3 worldNormal = aPos * mat3x3(transform.World);

	//ambient
	vec4 ambient = light.ambient + material.ambient;

	//diffuse
	vec3 dirToLight = -light.direction;
	float diffuseIntensity = clamp(dot(dirToLight, worldNormal),0.0,1.0);
	vec4 diffuse = diffuseIntensity * light.diffuse * material.diffuse;

	//specular
	vec3 dirToView = normalize(transform.ViewPosition -  worldPosition);
	vec3 halfAngle = normalize(dirToLight + dirToView);
	float specularBase = clamp(dot(halfAngle, worldNormal), 0.0,1.0);
	float specularIntensity = pow(specularBase, material.power);
	vec4 specular = specularIntensity * light.specular * material.specular;

	vec4 color = ambient + diffuse + specular;

	gl_Position = transform.WVP * vec4(aPos, 1.0);
	outColor = color;
}

#shader fragment
#version 330 core

out vec4 FragColor;
in vec4 outColor;

void main()
{
	FragColor = outColor;
}