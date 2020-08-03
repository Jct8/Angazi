#pragma once
#include "Model.h"

namespace Angazi::Graphics
{
	class Animator 
	{
	public:
		void Initialize(const Model& model);
		void Terminate();

		void ComputeBindPose();
		void SetClipLooping(int index, bool looping);
		void PlayAnimation(int index);
		void PlaySkeletalAnimation(int index);
		void BlendTo(int index, float duration);
		void Update(float deltaTime);
		void AddBlendAnimation(Math::Vector2 blendDirection, int clipNumber);


		void SetAnimationSpeed(float speed) { mAnimationSpeed = speed; };
		void SetBlendVelocity(Math::Vector2 velocity);
		void SetShowSkeleton(float show) { isSkeletalAnimation = show; };

		auto& GetBoneMatrices() const { return mBoneMatrices; }

	private:
		const Model* mModel;
		float mTimer = 0.0f;
		float mAnimationSpeed = 1.0f;
		int mClipIndex = 0;
		bool isSkeletalAnimation = false;

		float mBlendTimer = 0.0f;
		float mBlendWeight = 0.0f;
		float mBlendTime = 0.0f;
		float mBlendDuration = 0.0f;
		int mBlendIndex = -1;

		// <Angle, clipNumber>
		std::map<float, int> blendTree;

		std::vector<Angazi::Math::Matrix4> mBoneMatrices;
	};
}