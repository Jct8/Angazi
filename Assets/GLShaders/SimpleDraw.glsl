#shader VS
#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 WVP;

void main()
{
    gl_Position = WVP * vec4(aPos, 1.0);
	ourColor = aColor;
}

#shader PS
#version 400 core

out vec4 FragColor;
in vec3 ourColor;

void main()
{
	FragColor = vec4(ourColor, 1.0f);
}