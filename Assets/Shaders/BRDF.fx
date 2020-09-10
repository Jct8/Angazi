// Description: Simple shader that generates a 2D LUT from the BRDF equations
cbuffer TransformBuffer : register(b0)
{
	matrix WVP;
}

struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};

static const float PI = 3.14159265359f;
static const float EPSILON = 1e-6f;

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

float GeometrySchlickGGX(float nDotV, float roughness)
{
	float alpha = roughness;
	float k = (alpha * alpha) / 2.0;

	float denom = nDotV * (1.0 - k) + k;
	return nDotV / denom;
}
float GeometrySmith(float3 normal, float3 viewDir, float3 lightDir, float roughness)
{
	float nDotV = max(dot(normal, viewDir), 0.0f);
	float nDotL = max(dot(normal, lightDir), 0.0f);

	float ggx1 = GeometrySchlickGGX(nDotV,roughness);
	float ggx2 = GeometrySchlickGGX(nDotL,roughness);
	
	return ggx1 * ggx2;
}

float2 IntegrateBRDF(float nDotV, float roughness)
{
	float3 viewDir;
	viewDir.x = sqrt(1.0f - nDotV * nDotV);
	viewDir.y = 0.0f;
	viewDir.z = nDotV;

	float A = 0.0f;
	float B = 0.0f;

	float3 normal = float3(0.0, 0.0, 1.0);

	const uint numSamples = 1024;
	for (uint i = 0; i < numSamples; ++i)
	{
		float2 Xi = Hammersley(i, numSamples);
		float3 halfway = ImportanceSampleGGX(Xi, normal, roughness);
		float3 lightDir = normalize(2.0f * dot(viewDir, halfway) * halfway - viewDir);

		float nDotL = max(lightDir.z, 0.0f);
		float nDotH = max(halfway.z, 0.0f);
		float vDotH = max(dot(viewDir, halfway), 0.0f);

		if (nDotL > 0.0)
		{
			float G = GeometrySmith(normal, viewDir, lightDir, roughness);
			float G_Vis = (G * vDotH) / (nDotH * nDotV);
			float Fc = pow(1.0 - vDotH, 5.0);

			A += (1.0 - Fc) * G_Vis;
			B += Fc * G_Vis;
		}
	}
	A /= float(numSamples);
	B /= float(numSamples);
	return float2(A, B);
	return float2(A, B) / float(numSamples);
}

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.texCoord = input.texCoord;
	output.position = float4(input.position, 1.0f);
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float2 integratedBRDF = IntegrateBRDF(input.texCoord.x, input.texCoord.y);
	return float4(integratedBRDF,0.0f, 1.0);
}