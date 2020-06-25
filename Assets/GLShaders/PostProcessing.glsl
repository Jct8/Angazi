#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 outTexCoord;

layout (location = 0) uniform sampler2D textureMap;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    gl_Position = vec4(aPos, 1.0);
    outTexCoord = aTexCoord;
}

#shader PS
#version 450 core

in vec2 outTexCoord;
out vec4 FragColor;

layout (location = 0) uniform sampler2D textureMap;

void main()
{
	vec4 color = texture(textureMap, outTexCoord);

	// Blur
	color *= 0.6f;
	color += texture(textureMap, outTexCoord + (0.005)) * 0.4f;
	
	FragColor = color;
}

#shader PSNoProcessing
#version 450 core

in vec2 outTexCoord;
out vec4 FragColor;

layout (location = 0) uniform sampler2D textureMap;

void main()
{
	vec4 color = texture(textureMap, outTexCoord);
	FragColor = color;
}