#pragma once

#include "Component.h"

namespace Angazi
{
	class TransformComponent;
	class MaterialComponent;

	class SkinnedMeshComponent : public Component
	{
	public:
		META_CLASS_DECLARE;

		void Initialize() override;
		void Terminate() override;
		void Render() override;
		void RenderShadow() override;
		void Update(float deltaTime) override;
		void ShowInspectorProperties() override;

		void RenderEdge();
	private:
		void InitializeAnimator();

		const TransformComponent* mTransformComponent = nullptr;
		MaterialComponent* mMaterialComponent = nullptr;
		std::filesystem::path mModelFileName = "None";

		Graphics::ModelId mModelId;

		int currentAnimation = 0;
		Graphics::Animator animator;

		bool isPlayingAnimation = false;
		bool mShowSkeleton = false;
		bool mIsCastingShadow = true;
		bool mIsReceivingShadows = true;
	};
}