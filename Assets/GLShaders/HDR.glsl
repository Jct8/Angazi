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
	outTexCoord.y = -outTexCoord.y;
}

#shader PS
#version 450 core

layout(std140, binding = 0) uniform SettingsBuffer 
{
	bool useHDR;
	bool useGammaCorrection;
	float exposure;
};

in vec2 outTexCoord;
out vec4 FragColor;

layout (location = 0) uniform sampler2D textureMap;

void main()
{
	const float gamma = 2.2f;
	const float gammaInv = 1.0f/gamma;
	vec4 color = texture(textureMap, outTexCoord);
	if (useHDR)
		color = vec4(1.0f) - exp(-color * exposure);
	if (useGammaCorrection)	
		color = pow(abs(color), vec4(gammaInv));
	
	FragColor = color;
}
