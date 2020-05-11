#include "Precompiled.h"
#include "Animation.h"

using namespace Angazi;
using namespace Angazi::Math;
using namespace Angazi::Graphics;

Math::Vector3 Animation::GetPosition(float time) const
{
	if (mPositionKeyframes.back().time < time)
		if (mLooping)
			time = (std::fmodf(time ,GetDuration()));
		else
			return mPositionKeyframes.back().key;
	if (mPositionKeyframes.size() == 1)
		return mPositionKeyframes.back().key;

	//Calculation
	size_t i = 0;
	for (; i < mPositionKeyframes.size(); ++i)
	{
		if (time < mPositionKeyframes[i].time)
			break;
	}
	if (i == mPositionKeyframes.size())
		i--;
	float timeFrameDuration = time - mPositionKeyframes[i - 1].time;
	float totalFrameDuration = mPositionKeyframes[i].time - mPositionKeyframes[i - 1].time;
	float t = timeFrameDuration / totalFrameDuration;
	return Lerp(mPositionKeyframes[i - 1].key, mPositionKeyframes[i].key, t);
}

Math::Quaternion Animation::GetRotation(float time) const
{
	//Bound Checks
	if (mPositionKeyframes.back().time < time)
		if (mLooping)
			time = (std::fmodf(time, GetDuration())) ;
		else
			return mRotationKeyframes.back().key;
	if (mRotationKeyframes.size() == 1)
		return mRotationKeyframes.back().key;

	//Calculation
	size_t i = 0;
	for (; i < mRotationKeyframes.size(); ++i)
	{
		if (time < mRotationKeyframes[i].time)
			break;
	}
	if (i == mRotationKeyframes.size())
		i--;
	float timeFrameDuration = time - mRotationKeyframes[i - 1].time;
	float totalFrameDuration = mRotationKeyframes[i].time - mRotationKeyframes[i - 1].time;
	float t = timeFrameDuration / totalFrameDuration;
	return Slerp(mRotationKeyframes[i - 1].key, mRotationKeyframes[i].key, t);
}

Math::Vector3 Animation::GetScale(float time) const
{
	//Bound Checks
	if (mPositionKeyframes.back().time < time)
		if (mLooping)
			time = (std::fmodf(time, GetDuration()));
		else
			return mScaleKeyframes.back().key;
	if (mScaleKeyframes.size() == 1)
		return mScaleKeyframes.back().key;

	//Calculation
	size_t i = 0;
	for (; i < mScaleKeyframes.size(); ++i)
	{
		if (time < mScaleKeyframes[i].time)
			break;
	}
	if (i == mScaleKeyframes.size())
		i--;
	float timeFrameDuration = time - mScaleKeyframes[i - 1].time;
	float totalFrameDuration = mScaleKeyframes[i].time - mScaleKeyframes[i - 1].time;
	float t = timeFrameDuration / totalFrameDuration;
	return Lerp(mScaleKeyframes[i - 1].key, mScaleKeyframes[i].key, t);
}

Math::Matrix4 Animation::GetTransform(float time) const
{
	Vector3 position = GetPosition(time);
	Quaternion rotation = GetRotation(time);
	Vector3 scale = GetScale(time);

	auto matTrans = Matrix4::Translation({ position });
	auto matRot = Matrix4::RotationQuaternion(rotation);
	auto matWorld = Matrix4::Scaling(scale) * matRot * matTrans;

	return matWorld;
}

float Animation::GetDuration() const
{
	return mPositionKeyframes.back().time - mPositionKeyframes.front().time;
}
