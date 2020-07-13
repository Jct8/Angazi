// Description: Simple shader that changes Equirectangular to Cubemap

cbuffer TransformBuffer : register(b0)
{
	matrix WVP;
}

Texture2D equirectangularMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position	: POSITION;
};

struct VS_OUTPUT
{
	float4 position :		SV_Position;
	float3 worldPosition:	TEXCOORD;
};

static const float2 invAtan = float2(0.1591, 0.3183);
float2 SampleSphericalMap(float3 v)
{
	float2 uv = float2(atan2(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = mul(float4(input.position, 1.0f), WVP);
	output.worldPosition = input.position;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float2 uv = SampleSphericalMap(normalize(input.worldPosition));
	return equirectangularMap.Sample(textureSampler, uv);
}