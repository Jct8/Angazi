#pragma once

#include "Bone.h"
#include "AnimationClip.h"

namespace Angazi::Graphics
{
	struct Skeleton
	{
		Bone* root = nullptr;
		std::vector<std::unique_ptr<Bone>> bones;
	};

	void DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneMatrices, float scale = 1.0f);
	void UpdateBoneMatrices(Bone* bone, std::vector<Math::Matrix4>& boneMatrices, bool applyOffset = false, const AnimationClip& clip = AnimationClip(), float time = 0.0f);
	void UpdateBoneMatrices(Bone* bone, std::vector<Math::Matrix4>& boneMatrices, 
		const AnimationClip& clipFrom, const AnimationClip& clipTo, float blendWeight, bool applyOffset = false ,float time = 0.0f);
}