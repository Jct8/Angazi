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

VSOutput VS(VSInput input)
{
	VSOutput output;
	output.position = float4(input.position,1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	float4 color = textureMap.Sample(textureSampler,input.texCoord);
	color *= abs(cos(input.texCoord.y * 720));
	return float4(0,color.g,0,1);
}