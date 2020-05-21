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
	ASSERT(index <= mModel->animationSet.clips.size(), "[Animator] - Model does not have clip index.");
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
	ASSERT(index <= mModel->animationSet.clips.size(), "[Animator] - Model does not have clip index.");
	mClipIndex = index;
	isSkeletalAnimation = false;
}
void Animator::PlaySkeletalAnimation(int index)
{
	ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	ASSERT(index <= mModel->animationSet.clips.size(), "[Animator] - Model does not have clip index.");
	mClipIndex = index;
	isSkeletalAnimation = true;
}

void Animator::Update(float deltaTime)
{
	mTimer += mAnimationSpeed * deltaTime;
	ASSERT(mModel != nullptr, "[Animator] - Animator has no model.");
	UpdateBoneMatrices(mModel->skeleton.root, mBoneMatrices, !isSkeletalAnimation, *(mModel->animationSet.clips[mClipIndex]), mTimer);
}
