//Description: Shader for generating a depth map
cbuffer ConstantBuffer : register(b0)
{
	matrix wvp;
}

cbuffer BoneTransformBuffer : register(b1)
{
	matrix boneTransforms[256];
}

cbuffer Settings : register(b2)
{
	float isSkinnedMesh : packoffset(c0.x);
}

static matrix Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

matrix GetBoneTransform(int4 indices, float4 weights)
{
	if (length(weights) <= 0.0f || isSkinnedMesh == 0.0f)
		return Identity;

	matrix transform;
	transform = boneTransforms[indices[0]] * weights[0];
	transform += boneTransforms[indices[1]] * weights[1];
	transform += boneTransforms[indices[2]] * weights[2];
	transform += boneTransforms[indices[3]] * weights[3];
	return transform;
}

struct VS_INPUT
{
	float3 position : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float2 texCoord	: TEXCOORD;
	int4 blendIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHT;
};
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 positionNDC : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	matrix toRoot = GetBoneTransform(input.blendIndices, input.blendWeights);
	matrix toNDC = mul(toRoot, wvp);

	output.position = mul(float4(input.position, 1.0f), toNDC);
	output.positionNDC = output.position;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float depth = 1.0f - input.positionNDC.z / input.positionNDC.w;
	return float4(depth,1.0f,1.0f,1.0f);
}
