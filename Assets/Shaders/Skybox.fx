// Description: Simple shader that applies transformation and texturing.

cbuffer TransformBuffer : register(b0)
{
	matrix WVP;
}

TextureCube textureMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position	: POSITION;
	//float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float3 texCoord	: TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = mul(float4(input.position, 1.0f), WVP).xyww;
	output.texCoord = input.position;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	return textureMap.Sample(textureSampler, input.texCoord);
}