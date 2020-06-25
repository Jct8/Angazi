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
	output.position = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	float4 color = textureMap.Sample(textureSampler, input.texCoord);

	//Night Vision
	//color *= abs(cos(input.texCoord.y * 720));
	//return float4(0,color.g,0,1);
	
	//Gray Scale 
	//color *= dot(color.rgb, float3(0.3,0.4,0.6));
	//return float4(color.rrr, 1);
	
	// Blur
	color *= 0.6f;
	color += textureMap.Sample(textureSampler, input.texCoord + (0.005)) * 0.2f;
	return color;

	//for (int i = 0; i < 20; i++)
	//	color += textureMap.Sample(textureSampler, input.texCoord );
	//return color;
	//color *= textureMap.Sample(textureSampler, input.texCoord*0.4);
	return color;
}

float4 PSNoProcessing(VSOutput input) : SV_Target
{
	float4 color = textureMap.Sample(textureSampler, input.texCoord);
	return color;
}