#include "Precompiled.h"
#include "Animation.h"

using namespace Angazi;
using namespace Angazi::Math;
using namespace Angazi::Graphics;

Math::Vector3 Animation::GetPosition(float time) const
{
	if (mPositionKeyframes[mPositionKeyframes.size() - 1].time < time)
		if(mLooping)
			time -= GetDuration();
		else
			return mPositionKeyframes[mPositionKeyframes.size() - 1].key;
	int i = 0;
	for (; i < mPositionKeyframes.size(); ++i)
	{
		if (time < mPositionKeyframes[i].time)
			break;
	}
	float timeFrameDuration = time - mPositionKeyframes[i - 1].time;
	float totalFrameDuration = mPositionKeyframes[i].time - mPositionKeyframes[i - 1].time;
	float t = timeFrameDuration / totalFrameDuration;
	Vector3 position = Lerp(mPositionKeyframes[i - 1].key, mPositionKeyframes[i].key, t);
	return position;
}

Math::Quaternion Animation::GetRotation(float time) const
{
	if (mPositionKeyframes[mPositionKeyframes.size() - 1].time < time)
		if (mLooping)
			time -= GetDuration();
		else
			return mRotationKeyframes[mRotationKeyframes.size() - 1].key;

	int i = 0;
	for (; i < mRotationKeyframes.size(); ++i)
	{
		if (time < mRotationKeyframes[i].time)
			break;
		//if (!mLooping && i == mRotationKeyframes.size() - 1)
		//	return mRotationKeyframes[mRotationKeyframes.size() - 1].key;
	}
	float timeFrameDuration = time - mRotationKeyframes[i - 1].time;
	float totalFrameDuration = mRotationKeyframes[i].time - mRotationKeyframes[i - 1].time;
	float t = timeFrameDuration / totalFrameDuration;
	Quaternion rotation = Slerp(mRotationKeyframes[i - 1].key, mRotationKeyframes[i].key, t);
	return rotation;
}

Math::Vector3 Animation::GetScale(float time) const
{
	if (mPositionKeyframes[mPositionKeyframes.size() - 1].time < time)
		if (mLooping)
			time -= GetDuration();
		else
			return mScaleKeyframes[mScaleKeyframes.size() - 1].key;

	int i = 0;
	for (; i < mScaleKeyframes.size(); ++i)
	{
		if (time < mScaleKeyframes[i].time)
			break;
		if (i == mScaleKeyframes.size() - 1)
			return mScaleKeyframes[mScaleKeyframes.size() - 1].key;
	}
	float timeFrameDuration = time - mScaleKeyframes[i - 1].time;
	float totalFrameDuration = mScaleKeyframes[i].time - mScaleKeyframes[i - 1].time;
	float t = timeFrameDuration / totalFrameDuration;
	Vector3 scale = Lerp(mScaleKeyframes[i - 1].key, mScaleKeyframes[i].key, t);
	return scale;
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
