#include "Precompiled.h"
#include "AnimationIO.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void AnimationIO::Write(FILE * file, const Animation & animation)
{
	uint32_t totalPositionKeys = static_cast<uint32_t>(animation.mPositionKeyframes.size());
	fprintf_s(file, "PositionKeys: %d\n", totalPositionKeys);
	for (uint32_t i = 0; i < totalPositionKeys; i++)
		fprintf_s(file, "%f %f %f %f\n", animation.mPositionKeyframes[i].time , animation.mPositionKeyframes[i].key.x 
			, animation.mPositionKeyframes[i].key.y, animation.mPositionKeyframes[i].key.z);

	uint32_t totalRotationKeys = static_cast<uint32_t>(animation.mRotationKeyframes.size());
	fprintf_s(file, "RotationKeys: %d\n", totalRotationKeys);
	for (uint32_t i = 0; i < totalRotationKeys; ++i)
		fprintf_s(file, "%f %f %f %f %f\n", animation.mRotationKeyframes[i].time, animation.mRotationKeyframes[i].key.x
			, animation.mRotationKeyframes[i].key.y, animation.mRotationKeyframes[i].key.z, animation.mRotationKeyframes[i].key.w);

	uint32_t totalScaleKeys = static_cast<uint32_t>(animation.mScaleKeyframes.size());
	fprintf_s(file, "ScaleKeys: %d\n", totalScaleKeys);
	for (uint32_t i = 0; i < totalScaleKeys; i++)
		fprintf_s(file, "%f %f %f %f\n", animation.mScaleKeyframes[i].time, animation.mScaleKeyframes[i].key.x
			, animation.mScaleKeyframes[i].key.y, animation.mScaleKeyframes[i].key.z);

	fprintf_s(file, "Looping: %d\n", animation.IsLooping() ? 1 : 0);
}

void AnimationIO::Read(FILE * file, Animation & animation)
{
	uint32_t totalPositionKeys;
	fscanf_s(file, "PositionKeys: %d\n", &totalPositionKeys);
	animation.mPositionKeyframes.resize(totalPositionKeys);
	for (uint32_t i = 0; i < totalPositionKeys; i++)
		fscanf_s(file, "%f %f %f %f\n", &animation.mPositionKeyframes[i].time, &animation.mPositionKeyframes[i].key.x
			, &animation.mPositionKeyframes[i].key.y, &animation.mPositionKeyframes[i].key.z);

	uint32_t totalRotationKeys;
	fscanf_s(file, "RotationKeys: %d\n", &totalRotationKeys);
	animation.mRotationKeyframes.resize(totalRotationKeys);
	for (uint32_t i = 0; i < totalRotationKeys; ++i)
		fscanf_s(file, "%f %f %f %f %f\n", &animation.mRotationKeyframes[i].time, &animation.mRotationKeyframes[i].key.x
			, &animation.mRotationKeyframes[i].key.y, &animation.mRotationKeyframes[i].key.z, &animation.mRotationKeyframes[i].key.w);

	uint32_t totalScaleKeys;
	fscanf_s(file, "ScaleKeys: %d\n", &totalScaleKeys);
	animation.mScaleKeyframes.resize(totalScaleKeys);
	for (uint32_t i = 0; i < totalScaleKeys; i++)
		fscanf_s(file, "%f %f %f %f\n", &animation.mScaleKeyframes[i].time, &animation.mScaleKeyframes[i].key.x
			, &animation.mScaleKeyframes[i].key.y, &animation.mScaleKeyframes[i].key.z);

	uint32_t isLooping;
	fscanf_s(file, "Looping: %d\n", &isLooping);
	animation.mLooping = isLooping == 1 ? true : false;
}

void AnimationIO::Write(FILE * file, const AnimationClip & animationClip)
{
	// Homework
	// Note that animationClip has the same number of animations as 
	// there are bones. This is so that we can use the bone index
	// to index for an animation directly. If animationClip.boneAnimations[i]
	// is null, you will still need to write out something like [Empty],
	// so when you load the animation clip, you know to skip those slots
	//
	// boneAnimations:
	// [--->][null][--->][--->][--->][null]
	//
	// output:
	// Animation: ...
	// [Empty]
	// Animation: ...
	// Animation: ...
	// Animation: ...
	// [Empty]

	fprintf_s(file, "Name: %s\n", animationClip.name.c_str());
	fprintf_s(file, "Duration: %f\n", animationClip.duration);
	fprintf_s(file, "TicksPerSecond: %f\n", animationClip.ticksPerSecond);
	uint32_t totalAnimations = static_cast<uint32_t>(animationClip.boneAnimations.size());
	fprintf_s(file, "AnimationCount: %d\n", totalAnimations);

	for (uint32_t i = 0; i < totalAnimations; i++)
	{
		Animation* boneAnimation = animationClip.boneAnimations[i].get();
		if (boneAnimation)
		{
			fprintf_s(file, "[Animation]\n");
			Write(file, *boneAnimation);
		}
		else
		{
			fprintf_s(file, "[Empty]\n");
		}
	}
}

void AnimationIO::Read(FILE * file, AnimationClip & animationClip)
{
	char buffer[128];
	fscanf_s(file, "Name: %s\n", buffer, std::size(buffer));
	animationClip.name = buffer;

	fscanf_s(file, "Duration: %f\n", &animationClip.duration);
	fscanf_s(file, "TicksPerSecond: %f\n", &animationClip.ticksPerSecond);
	uint32_t totalAnimations;
	fscanf_s(file, "AnimationCount: %d\n", &totalAnimations);

	animationClip.boneAnimations.resize(totalAnimations);
	for (uint32_t i = 0; i < totalAnimations; i++)
	{
		fscanf_s(file, "%s\n", buffer, std::size(buffer));
		if (std::string(buffer) == "[Animation]")
		{
			animationClip.boneAnimations[i] = std::make_unique<Animation>();
			Read(file,*animationClip.boneAnimations[i]);
		}
	}
}
