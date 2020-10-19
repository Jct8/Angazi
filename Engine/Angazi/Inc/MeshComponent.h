#pragma once

#include "Component.h"

namespace Angazi
{
	class TransformComponent;
	class MaterialComponent;

	class MeshComponent : public Component
	{
	public:
		META_CLASS_DECLARE;

		void Initialize() override;
		void Render() override;
		void RenderShadow() override;
		void ShowInspectorProperties() override;

	private:
		const TransformComponent* mTransformComponent = nullptr;
		const MaterialComponent* mMaterialComponent = nullptr;
		std::filesystem::path mMeshFileName = "None";
		std::vector<Graphics::MeshId> mMeshIds;

		bool mIsCastingShadow = false;
		bool mIsReceivingShadows= false;
	};
}