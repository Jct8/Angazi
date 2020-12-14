//Description: Simple shader that does coloring


struct VSInput
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};

cbuffer SettingsBuffer : register(b0)
{
	bool useHDR : packoffset(c0.x);
	bool useGammaCorrection : packoffset(c0.y);
	bool useEdgeDetection : packoffset(c0.z);
	float exposure : packoffset(c0.w);
	float width : packoffset(c1.x);
	float height : packoffset(c1.y);
}

Texture2D textureMap : register(t0);
Texture2D edgeMap : register(t1);
SamplerState textureSampler : register(s0);

VSOutput VS(VSInput input)
{
	VSOutput output;
	output.position = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	const float gamma = 2.2f;
	const float gammaInv = 1.0f/gamma;
	float4 color = textureMap.Sample(textureSampler, input.texCoord);
	if (useHDR)
		color = float4(1.0f, 1.0f, 1.0f, 1.0f) - exp(-color * exposure);
		//color = color / (color + float4(1.0f, 1.0f, 1.0f, 1.0f));
	if (useGammaCorrection)
		color = pow(abs(color), float4(gammaInv, gammaInv, gammaInv, gammaInv));

	if (useEdgeDetection)
	{
		float4 n[9];
		float w = 2.0 / width;
		float h = 2.0 / height;

		n[0] = edgeMap.Sample(textureSampler, input.texCoord + float2(-w, -h));// texture2D(tex, coord + vec2(-w, -h));
		n[1] = edgeMap.Sample(textureSampler, input.texCoord + float2(0.0, -h));// texture2D(tex, coord + vec2(0.0, -h));
		n[2] = edgeMap.Sample(textureSampler, input.texCoord + float2(w, -h));	// texture2D(tex, coord + vec2(w, -h));
		n[3] = edgeMap.Sample(textureSampler, input.texCoord + float2(-w, 0.0));// texture2D(tex, coord + vec2(-w, 0.0));
		n[4] = edgeMap.Sample(textureSampler, input.texCoord);					// texture2D(tex, coord);
		n[5] = edgeMap.Sample(textureSampler, input.texCoord + float2(w, 0.0));	// texture2D(tex, coord + vec2(w, 0.0));
		n[6] = edgeMap.Sample(textureSampler, input.texCoord + float2(-w, h));	// texture2D(tex, coord + vec2(-w, h));
		n[7] = edgeMap.Sample(textureSampler, input.texCoord + float2(0.0, h));	// texture2D(tex, coord + vec2(0.0, h));
		n[8] = edgeMap.Sample(textureSampler, input.texCoord + float2(w, h));	// texture2D(tex, coord + vec2(w, h));

		float4 sobel_edge_h = n[2] + (2.0 * n[5]) + n[8] - (n[0] + (2.0 * n[3]) + n[6]);
		float4 sobel_edge_v = n[0] + (2.0 * n[1]) + n[2] - (n[6] + (2.0 * n[7]) + n[8]);
		float4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));
		return color + sobel;
	}

	return color;
}