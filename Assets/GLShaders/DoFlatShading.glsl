#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoord;

out vec4 outColor;

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

void main()
{
	vec3 worldPosition = (vec4(aPos, 1.0f) * World).xyz;
	vec3 worldNormal =  aNormal.xyz * mat3x3(World);

	//ambient
	vec4 ambient = LightAmbient* MaterialAmbient;

	//diffuse
	vec3 dirToLight = -LightDirection;
	float diffuseIntensity = clamp(dot(dirToLight, worldNormal), 0.0 ,1.0);
	vec4 diffuse = diffuseIntensity * LightDiffuse * MaterialDiffuse;

	//specular
	vec3 dirToView = normalize(ViewPosition - worldPosition);
	vec3 halfAngle = normalize(dirToLight + dirToView);
	float specularBase = clamp(dot(halfAngle, worldNormal), 0.0,1.0);
	float specularIntensity = pow(specularBase, MaterialPower);
	vec4 specular = specularIntensity* LightSpecular* MaterialSpecular;

	vec4 color = ambient + diffuse + specular;

	gl_Position = vec4(aPos, 1.0)*WVP;
	outColor = color;
}

#shader PS
#version 450 core

flat in vec4 outColor;
out vec4 FragColor;

void main()
{
	FragColor = outColor;
}