#pragma once

#include "Keyframe.h"

namespace Angazi::Graphics
{
	class Animation
	{
	public:
		Animation() = default;
		Animation(PositionKeys positionKeys, RotationKeys rotationKeys, ScaleKeys scaleKeys, bool looping)
			: mPositionKeyframes(positionKeys), mRotationKeyframes(rotationKeys), mScaleKeyframes(scaleKeys), mLooping(looping) {}

		Math::Vector3 GetPosition(float time) const;
		Math::Quaternion GetRotation(float time) const;
		Math::Vector3 GetScale(float time) const;
		Math::Matrix4 GetTransform(float time) const;
		std::tuple<Math::Vector3, Math::Quaternion, Math::Vector3> GetTransformTuple(float time) const;

		float GetDuration() const;

		void SetLooping(bool looping) { mLooping = looping; }
		bool IsLooping() const { return mLooping; }

	private:
		friend class AnimationIO;
		PositionKeys mPositionKeyframes;
		RotationKeys mRotationKeyframes;
		ScaleKeys mScaleKeyframes;
		bool mLooping = false;
	};

	using Animations = std::vector<std::unique_ptr<Animation>>;
}