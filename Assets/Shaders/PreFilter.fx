// Description: Simple shader that prefilters a cubemap

cbuffer TransformBuffer : register(b0)
{
	matrix WVP;
}

cbuffer Settings : register(b1)
{
	float roughness : packoffset(c0.x);
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
static const float EPSILON = 1e-6f;

float DistributionGGX(float3 normal, float3 halfVector, float roughness)
{
	float alpha = roughness * roughness;
	float alphaSquared = alpha * alpha;
	float nDotH = max(dot(normal, halfVector), 0.0f);
	
	float denominator = (nDotH * nDotH) * (alphaSquared - 1.0f) + 1.0f;
	return alphaSquared / max(PI* denominator * denominator, EPSILON);
	//https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/ //
}

float RadicalInverse_VdC(uint bits)
{
	// VanDerCorpus calculation.
	// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html

	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

float2 Hammersley(uint i, uint n)
{
	return float2(float(i) / float(n), RadicalInverse_VdC(i));
}
float3 ImportanceSampleGGX(float2 Xi, float3 normal, float roughness)
{
	float alpha = roughness * roughness;
	
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (alpha * alpha - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	
	// Spherical to Cartesian 
	float3 halfway;
	halfway.x = cos(phi) * sinTheta;
	halfway.y = sin(phi) * sinTheta;
	halfway.z = cosTheta;
	
	// Tangent-space Halfway vector to World-Space sample vector
	float3 up = abs(normal.z) < 0.999 ? float3(0.0, 0.0, 1.0) : float3(1.0, 0.0, 0.0);
	float3 tangent = normalize(cross(up, normal));
	float3 bitangent = cross(normal, tangent);
	
	return normalize((tangent * halfway.x) + (bitangent * halfway.y) + (normal * halfway.z));
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
	float3 normal = normalize(input.worldPosition);
	
	float3 view = normal;
	
	const uint numSamples = 1024;
	float3 prefilteredColor = float3(0.0f, 0.0f, 0.0f);
	float totalWeight = 0.0f;

	for (uint i = 0; i < numSamples; ++i)
	{
		float2 xi = Hammersley(i, numSamples);
		float3 halfway = ImportanceSampleGGX(xi, normal, roughness);
		float3 lightDir = normalize(2.0 * dot(view, halfway) * halfway - view);

		float nDotL = max(dot(normal, lightDir), 0.0);
		if (nDotL > 0.0)
		{
			// sample from the environment's mip level based on roughness/pdf
			float D = DistributionGGX(normal, halfway, roughness);
			float nDotH = max(dot(normal, halfway), 0.0);
			float hDotV = max(dot(halfway, view), 0.0);
			float pdf = D * nDotH / (4.0 * hDotV) + 0.0001; // Probability density function

			float resolution = 1024.0; // Resolution of source cubemap (per face)
			float saTexel = 4.0 * PI / (6.0 * resolution * resolution);
			float saSample = 1.0 / (float(numSamples) * pdf + 0.0001);

			float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

			lightDir.y = -lightDir.y; // Flip image
			prefilteredColor += environmentMap.SampleLevel(textureSampler, lightDir, mipLevel).rgb * nDotL;
			totalWeight += nDotL;
		}
	}

	prefilteredColor = prefilteredColor / totalWeight;

	return float4(prefilteredColor, 1.0);
}