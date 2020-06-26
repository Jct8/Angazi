#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 outColor;

out gl_PerVertex
{
    vec4 gl_Position;
};

layout(std140 , binding = 0) uniform Transforms
{
	mat4 WVP;
};

void main()
{
    gl_Position = vec4(aPos, 1.0) * WVP;
    outColor = aColor;
}

#shader PS
#version 450 core

out vec4 FragColor;
in vec4 outColor;

void main()
{
	FragColor = outColor;
}