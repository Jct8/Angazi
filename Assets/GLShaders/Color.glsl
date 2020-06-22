#shader VS
#version 450 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;

out vec4 ourColor;

void main()
{
    gl_Position =  aPos;//vec4(aPos, 1.0);
    ourColor = aColor;
}

#shader PS
#version 450 core

in vec4 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = ourColor;
}