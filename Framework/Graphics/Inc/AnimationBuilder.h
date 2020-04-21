#pragma once

#include "Animation.h"

namespace Angazi::Graphics
{
	class AnimationBuilder
	{
	public:
		AnimationBuilder& SetLooping(bool looping);

		AnimationBuilder& SetTime(float time);
		AnimationBuilder& AddPositionKey(const Math::Vector3& position);
		AnimationBuilder& AddRotationKey(const Math::Quaternion& rotation);
		AnimationBuilder& AddScaleKey(const Math::Vector3& scale);

		AnimationBuilder& AddPositionKey(const Math::Vector3& position, float time);
		AnimationBuilder& AddRotationKey(const Math::Quaternion& rotation, float time);
		AnimationBuilder& AddScaleKey(const Math::Vector3& scale, float time);

		AnimationBuilder& AddKey(Math::Vector3 position, Math::Quaternion rotation, float time);
		AnimationBuilder& AddKey(Math::Vector3 position, Math::Quaternion rotation, Math::Vector3 scale, float time);

		Animation Build();
	private:
		PositionKeys mPositionKeyframes;
		RotationKeys mRotationKeyframes;
		ScaleKeys mScaleKeyframes;

		float mTime = 0.0f;
		bool mLooping = false;
	};
}