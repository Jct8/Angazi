#include "Precompiled.h"
#include "Animator.h"
#include "Model.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Animator::Initialize(const Model & model)
{
	mModel = &model;
	mBoneMatrices.clear();
	mBoneMatrices.resize(mModel->skeleton.bones.size());
	isSkeletalAnimation = false;
	ComputeBindPose();
}
void Animator::Terminate()
{
	isSkeletalAnimation = false;
	mTimer = 0.0f;
	mClipIndex = 0;
	mAnimationSpeed = 10.0f;
	mBoneMatrices.clear();
	mModel = nullptr;
}

void Animator::ComputeBindPose()
{
	ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	UpdateBoneMatrices(mModel->skeleton.root, mBoneMatrices);
}
void Animator::SetClipLooping(int index, bool looping)
{
	ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	ASSERT(index < static_cast<int>(mModel->animationSet.clips.size()), "[Animator] - Model does not have clip index.");
	for (size_t i = 0; i < mModel->animationSet.clips[index]->boneAnimations.size(); i++)
	{
		if (mModel->animationSet.clips[index]->boneAnimations[i])
		{
			mModel->animationSet.clips[index]->boneAnimations[i]->SetLooping(looping);
		}
	}
}
void Animator::PlayAnimation(int index)
{
	ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	ASSERT(index < static_cast<int>(mModel->animationSet.clips.size()), "[Animator] - Model does not have clip index.");
	mClipIndex = index;
	isSkeletalAnimation = false;
	mBlendDuration = 0.0f;
}
void Animator::PlaySkeletalAnimation(int index)
{
	ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	ASSERT(index < static_cast<int>(mModel->animationSet.clips.size()), "[Animator] - Model does not have clip index.");
	mClipIndex = index;
	isSkeletalAnimation = true;
	mBlendDuration = 0.0f;
}

void Animator::BlendTo(int index, float duration)
{
	ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	ASSERT(index < static_cast<int>(mModel->animationSet.clips.size()), "[Animator] - Model does not have clip index.");
	if (mBlendIndex == index)
		return;
	mBlendWeight = 0.0f;
	mBlendTime = 0.0f;
	mBlendDuration = duration;
	mBlendIndex = index;
}
void Animator::Update(float deltaTime)
{
	ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	AnimationClip* clip = mModel->animationSet.clips[mClipIndex].get();
	mTimer += mAnimationSpeed * clip->ticksPerSecond *0.5f * deltaTime;
	UpdateBoneMatrices(mModel->skeleton.root, mBoneMatrices, !isSkeletalAnimation, *clip, mTimer);

	if (mBlendDuration > 0.0f)
	{
		auto& blendClip = mModel->animationSet.clips[mBlendIndex];
		mBlendTimer += deltaTime * mAnimationSpeed;

		mBlendTime += deltaTime;
		if (mBlendTime > mBlendDuration)
		{
			mClipIndex = mBlendIndex;
			mTimer = mBlendTimer;
			mBlendWeight = 1.0f;
			mBlendDuration = 0.0f;
		}
		else
		{
			mBlendWeight = mBlendTime / mBlendDuration;
		}

		std::vector<Angazi::Math::Matrix4> targetMatrices(mBoneMatrices.size());

		UpdateBoneMatrices(mModel->skeleton.root, targetMatrices, !isSkeletalAnimation, *blendClip, mBlendTimer);

		for (size_t i = 0; i < mBoneMatrices.size(); i++)
		{
			mBoneMatrices[i] = mBoneMatrices[i] * (1.0f - mBlendWeight) + targetMatrices[i] * mBlendWeight;
		}

	}
}
