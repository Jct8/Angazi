#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 ourColor;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    gl_Position =  vec4(aPos,1.0f);
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