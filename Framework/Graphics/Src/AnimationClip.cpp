#include "Precompiled.h"
#include "AnimationClip.h"

using namespace Angazi;
using namespace Angazi::Graphics;

bool AnimationClip::GetTransform(float time, uint32_t boneIndex, Math::Matrix4 & transform) const
{
	auto& boneAnim = boneAnimations[boneIndex];
	if (boneAnim)
	{
		transform = boneAnim->GetTransform(time);
		return true;
	}
	return false;
}

bool AnimationClip::GetTransformTuple(float time, uint32_t boneIndex, std::tuple<Math::Vector3, Math::Quaternion, Math::Vector3> & transform) const
{
	auto& boneAnim = boneAnimations[boneIndex];
	if (boneAnim)
	{
		transform = boneAnim->GetTransformTuple(time);
		return true;
	}
	return false;
}