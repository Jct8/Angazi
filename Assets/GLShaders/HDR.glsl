#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 outTexCoord;

layout (binding = 0) uniform sampler2D textureMap;
layout (binding = 1) uniform sampler2D edgeMap;

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
	bool useEdgeDetection;
	float exposure;
	float width;
	float height;
};

in vec2 outTexCoord;
out vec4 FragColor;

layout (binding = 0) uniform sampler2D textureMap;
layout (binding = 1) uniform sampler2D edgeMap;

void main()
{
	const float gamma = 2.2f;
	const float gammaInv = 1.0f/gamma;
	vec4 color = texture(textureMap, outTexCoord);
	if (useHDR)
		color = vec4(1.0f) - exp(-color * exposure);
	if (useGammaCorrection)	
		color = pow(abs(color), vec4(gammaInv));

	if (useEdgeDetection)
	{
		vec4 n[9];
		float w = 2.0 / width;
		float h = 2.0 / height;

		n[0] = texture(edgeMap, outTexCoord + vec2(-w, -h));
		n[1] = texture(edgeMap, outTexCoord + vec2(0.0, -h));
		n[2] = texture(edgeMap, outTexCoord + vec2(w, -h));
		n[3] = texture(edgeMap, outTexCoord + vec2(-w, 0.0));
		n[4] = texture(edgeMap, outTexCoord);
		n[5] = texture(edgeMap, outTexCoord + vec2(w, 0.0));
		n[6] = texture(edgeMap, outTexCoord + vec2(-w, h));
		n[7] = texture(edgeMap, outTexCoord + vec2(0.0, h));
		n[8] = texture(edgeMap, outTexCoord + vec2(w, h));

		vec4 sobel_edge_h = n[2] + (2.0 * n[5]) + n[8] - (n[0] + (2.0 * n[3]) + n[6]);
		vec4 sobel_edge_v = n[0] + (2.0 * n[1]) + n[2] - (n[6] + (2.0 * n[7]) + n[8]);
		vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));
		color += sobel;
	}

	FragColor = color;
}
