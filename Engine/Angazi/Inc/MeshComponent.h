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

		void Terminate() override;

	private:
		const TransformComponent* mTransformComponent = nullptr;
		const MaterialComponent* mMaterialComponent = nullptr;
		std::string mMeshFileName;
		Graphics::MeshId mMeshId = 0;
	};
}