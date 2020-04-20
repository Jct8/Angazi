#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace Angazi;
using namespace Angazi::Graphics;

AnimationBuilder & AnimationBuilder::SetLooping(bool looping)
{
	mLooping = looping;
	return *this;
}

AnimationBuilder& AnimationBuilder::SetTime(float time)
{
	mTime = time;
	return *this;
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const Math::Vector3 & position)
{
	mPositionKeyframes.push_back({ position, mTime });
	return *this;
}

AnimationBuilder& AnimationBuilder::AddRotationKey(const Math::Quaternion & rotation)
{
	mRotationKeyframes.push_back({ rotation, mTime });
	return *this;
}

AnimationBuilder& AnimationBuilder::AddScaleKey(const Math::Vector3 & scale)
{
	mScaleKeyframes.push_back({ scale , mTime });
	return *this;
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const Math::Vector3 & position, float time)
{
	mPositionKeyframes.push_back({ position,time });
	return *this;
}

AnimationBuilder& AnimationBuilder::AddRotationKey(const Math::Quaternion & rotation, float time)
{
	mRotationKeyframes.push_back({ rotation,time });
	return *this;
}

AnimationBuilder& AnimationBuilder::AddScaleKey(const Math::Vector3 & scale, float time)
{
	mScaleKeyframes.push_back({ scale ,time });
	return *this;
}

AnimationBuilder& AnimationBuilder::AddKey(Math::Vector3 position, Math::Quaternion rotation, float time)
{
	mPositionKeyframes.push_back({ position, time });
	mRotationKeyframes.push_back({ rotation, time });
	return *this;
}

Animation AnimationBuilder::Build()
{
	return Animation(mPositionKeyframes,mRotationKeyframes,mScaleKeyframes,mLooping);
}
