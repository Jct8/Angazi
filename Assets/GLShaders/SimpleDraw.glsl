#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
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
    gl_Position =  vec4(aPos, 1.0) * WVP;
	ourColor = aColor;
}

#shader PS
#version 450 core

out vec4 FragColor;
in vec3 ourColor;

void main()
{
	FragColor = vec4(ourColor, 1.0f);
}