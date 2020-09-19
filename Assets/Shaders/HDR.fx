//Description: Simple shader that does coloring

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

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
	float exposure : packoffset(c0.z);
}

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
	
	return color;
}