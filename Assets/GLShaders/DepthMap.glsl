#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in uvec4 aBlendIndices;
layout (location = 5) in vec4 aBlendWeights;

out vec4 outPositionNDC;

out gl_PerVertex
{
	vec4 gl_Position;
};

layout(std140, binding = 0) uniform ConstantBuffer
{
	mat4 wvp;
};

layout(std140, binding = 1) uniform BoneTransformBuffer 
{
	mat4x4 boneTransforms[256];
};

layout(std140, binding = 2) uniform Settings 
{
	float isSkinnedMesh;
};

mat4 Identity = mat4
	(1.0f, 0.0f, 0.0f, 0.0f,
	 0.0f, 1.0f, 0.0f, 0.0f,
	 0.0f, 0.0f, 1.0f, 0.0f,
	 0.0f, 0.0f, 0.0f, 1.0f); 

mat4 GetBoneTransform(uvec4 indices, vec4 weights)
{
	if (length(weights) <= 0.0f || isSkinnedMesh == 0.0f)
		return Identity;

	mat4 transform;
	transform = boneTransforms[indices[0]] * weights[0];
	transform += boneTransforms[indices[1]] * weights[1];
	transform += boneTransforms[indices[2]] * weights[2];
	transform += boneTransforms[indices[3]] * weights[3];
	return transform;
}

void main()
{
	mat4 toRoot = GetBoneTransform(aBlendIndices, aBlendWeights);
	mat4 toNDC = toRoot * wvp;

	vec4 outPos = vec4(aPos, 1.0f) * toNDC;
	outPositionNDC = outPos;

	gl_Position = outPos;
}

#shader PS
#version 450 core

in vec4 outPositionNDC;
out vec4 FragColor;

void main()
{
	float depth = 1.0f - outPositionNDC.z / outPositionNDC.w;
	FragColor = vec4(depth,1.0f,1.0f,1.0f);
}