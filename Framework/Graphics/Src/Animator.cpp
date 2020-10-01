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
	//ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	if (!mModel)
		return;
	UpdateBoneMatrices(mModel->skeleton.root, mBoneMatrices,!isSkeletalAnimation);
}
void Animator::SetClipLooping(int index, bool looping)
{
	//ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	if (!mModel)
		return;
	ASSERT(index < static_cast<int>(mModel->animationSet.clips.size()), "[Animator] - Model does not have clip index.");
	for (size_t i = 0; i < mModel->animationSet.clips[index]->boneAnimations.size(); i++)
	{
		if (mModel->animationSet.clips[index]->boneAnimations[i])
			mModel->animationSet.clips[index]->boneAnimations[i]->SetLooping(looping);
	}
}
void Animator::PlayAnimation(int index)
{
	//ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	if (!mModel)
		return;
	ASSERT(index < static_cast<int>(mModel->animationSet.clips.size()), "[Animator] - Model does not have clip index.");
	mClipIndex = index;
	isSkeletalAnimation = false;
	mBlendDuration = 0.0f;
}
void Animator::PlaySkeletalAnimation(int index)
{
	//ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	if (!mModel)
		return;
	ASSERT(index < static_cast<int>(mModel->animationSet.clips.size()), "[Animator] - Model does not have clip index.");
	mClipIndex = index;
	isSkeletalAnimation = true;
	mBlendDuration = 0.0f;
}

void Animator::BlendTo(int index, float duration)
{
	//ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	if (!mModel)
		return;
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
	//ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	if (!mModel)
		return;
	AnimationClip* clip = mModel->animationSet.clips[mClipIndex].get();
	mTimer += mAnimationSpeed * clip->ticksPerSecond *0.5f * deltaTime;

	if (mBlendDuration > 0.0f && mBlendIndex!= -1)
	{
		auto& blendClip = mModel->animationSet.clips[mBlendIndex];
		mBlendTimer += deltaTime * mAnimationSpeed;

		mBlendTime += deltaTime;
		if (mBlendTime > mBlendDuration)
		{
			mClipIndex = mBlendIndex;
			mBlendIndex = -1;
			mTimer = mBlendTimer;
			mBlendWeight = 1.0f;
			mBlendDuration = 0.0f;
		}
		else
		{
			mBlendWeight = mBlendTime / mBlendDuration;
		}
		UpdateBoneMatrices(mModel->skeleton.root, mBoneMatrices, *clip, *blendClip, mBlendWeight, !isSkeletalAnimation, mBlendTimer);
		//std::vector<Angazi::Math::Matrix4> targetMatrices(mBoneMatrices.size());
		/*for (size_t i = 0; i < mBoneMatrices.size(); i++)
		{
			mBoneMatrices[i] = mBoneMatrices[i] * (1.0f - mBlendWeight) + targetMatrices[i] * mBlendWeight;
		}*/
		if (mBlendWeight == 1.0f)
			mBlendWeight = 0.0f;
	}
	else if (mBlendWeight > 0 && mBlendIndex != -1)
	{
		auto& blendClip = mModel->animationSet.clips[mBlendIndex];
		UpdateBoneMatrices(mModel->skeleton.root, mBoneMatrices, *clip, *blendClip , mBlendWeight, !isSkeletalAnimation, mTimer);
	}
	else
	{
		UpdateBoneMatrices(mModel->skeleton.root, mBoneMatrices, !isSkeletalAnimation, *clip, mTimer);
	}
}

void Animator::SetBlendVelocity(Math::Vector2 velocity)
{
	ASSERT(blendTree.size() > 2, "Animator - Blend tree must be larger than two.");
	float angle = atan2(velocity.y, velocity.x);

	if (velocity.y < 0.0f)
		angle += Math::Constants::TwoPi;
	std::map<float, int>::iterator currentAngle = blendTree.begin();
	for (; currentAngle != blendTree.end(); ++currentAngle)
	{
		if (angle < currentAngle->first)
		{
			auto prevAngle = currentAngle == blendTree.begin() ? --blendTree.end() : std::prev(currentAngle);
			mBlendWeight = (angle - prevAngle->first) / (currentAngle->first - prevAngle->first);
			mClipIndex = prevAngle->second;
			mBlendIndex = currentAngle->second;
			return;
		}
		if (angle == currentAngle->first)
		{
			//BlendTo(currentAngle->second,0.3f);
			mClipIndex = currentAngle->second;
			mBlendWeight = 0.0f;
			mBlendTime = 0.0f;
			mBlendDuration = 0.0f;
			mBlendIndex = -1;
			return;
		}
	}
	auto prevAngle =  --blendTree.end();
	currentAngle = blendTree.begin();
	mBlendWeight = (angle - prevAngle->first) / (Math::Constants::TwoPi- prevAngle->first);
	mClipIndex = prevAngle->second;
	mBlendIndex = currentAngle->second;
}

void Animator::AddBlendAnimation(Math::Vector2 blendDirection, int clipNumber)
{
	float angle = atan2(blendDirection.y,blendDirection.x);
	if (blendDirection.y < 0.0f)
		angle += Math::Constants::TwoPi;
	blendTree[angle] = clipNumber;
}
