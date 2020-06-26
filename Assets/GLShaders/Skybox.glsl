#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
out vec3 TexCoord;

out gl_PerVertex
{
    vec4 gl_Position;
};

layout(std140, binding = 0) uniform TransformBuffer
{
	mat4x4 WVP;
};

layout(binding = 0) uniform samplerCube skyboxMap;

void main()
{
    gl_Position = (vec4(aPos, 1.0) *WVP).xyww;
    TexCoord = aPos;
}

#shader PS
#version 450 core

in vec3 TexCoord;
out vec4 FragColor;

layout(std140, binding = 0) uniform TransformBuffer
{
	mat4x4 WVP;
};

layout(binding = 0) uniform samplerCube skyboxMap;


void main()
{
	FragColor = texture(skyboxMap, TexCoord);
}