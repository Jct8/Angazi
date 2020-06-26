#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

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
    gl_Position = vec4(aPos, 1.0)*  WVP;
    TexCoord = aTexCoord;
}

#shader PS
#version 450 core

out vec4 FragColor;
in vec2 TexCoord;

layout(binding = 0) uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord);
}