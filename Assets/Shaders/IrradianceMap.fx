// Description: Simple shader that convolutes a cubemap

cbuffer TransformBuffer : register(b0)
{
	matrix WVP;
}

TextureCube environmentMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float3 worldPosition : TEXCOORD;
};

static const float PI = 3.14159265359f;

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = mul(float4(input.position, 1.0f), WVP);
	output.worldPosition = input.position;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 normal = normalize(input.worldPosition);
	float3 irradiance = float3(0.0f, 0.0f, 0.0f);

	float3 right = cross(float3(0.0f, 1.0f, 0.0f), normal);
	float3 up = cross(normal, right);

	float sampleDelta = 0.025f;
	float numSamples = 0.0f;

	for (float phi = 0.0f; phi < 2.0f * PI; phi += sampleDelta)
	{
		for (float theta = 0.0f; theta < 0.5f * PI; theta += sampleDelta)
		{
			float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
			sampleVec.y = -sampleVec.y;
			irradiance += environmentMap.Sample(textureSampler, sampleVec).rgb * cos(theta) * sin(theta);
			numSamples++;
		}
	}
	irradiance *= PI / numSamples;
	return float4(irradiance, 1.0f);
}