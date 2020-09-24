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
		void Render() override;
		void Terminate() override;

		void ShowInspectorProperties() override;

	private:
		const TransformComponent* mTransformComponent = nullptr;
		const MaterialComponent* mMaterialComponent = nullptr;
		std::filesystem::path mModelFileName;
		Graphics::ModelId mModelId;

		Graphics::StandardEffect mModelStandardEffect;
	};
}