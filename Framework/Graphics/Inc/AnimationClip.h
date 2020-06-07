#pragma once

#include "Animation.h"

namespace Angazi::Graphics
{
	class AnimationClip
	{
	public:
		bool GetTransform(float time, uint32_t boneIndex, Math::Matrix4& transform) const;
		bool GetTransformTuple(float time, uint32_t boneIndex, std::tuple<Math::Vector3, Math::Quaternion, Math::Vector3>& transform) const;

		std::string name;
		float duration = 0.0f;
		float ticksPerSecond = 0.0f;
		Animations boneAnimations;
	};
}