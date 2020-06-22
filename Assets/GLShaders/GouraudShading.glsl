#shader VS
#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aNormal;

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
	vec3 worldPosition = (transform.World* vec4(aPos, 1.0f)).xyz;
	vec3 worldNormal =  mat3x3(transform.World) * aNormal.xyz;

	//ambient
	vec4 ambient = light.ambient * material.ambient;

	//diffuse
	vec3 dirToLight = -light.direction;
	float diffuseIntensity = clamp(dot(dirToLight, worldNormal), 0.0 ,1.0);
	vec4 diffuse = diffuseIntensity * light.diffuse * material.diffuse ;

	//specular
	vec3 dirToView = normalize(transform.ViewPosition - worldPosition);
	vec3 halfAngle = normalize(dirToLight + dirToView);
	float specularBase = clamp(dot(halfAngle, worldNormal), 0.0,1.0);
	float specularIntensity = pow(specularBase, material.power);
	vec4 specular =specularIntensity* material.specular * light.specular ;

	vec4 color = ambient + diffuse + specular;

	gl_Position = transform.WVP * vec4(aPos, 1.0);
	outColor = color;
}

#shader PS
#version 400 core

in vec4 outColor;
out vec4 FragColor;

void main()
{
	FragColor = outColor;
}