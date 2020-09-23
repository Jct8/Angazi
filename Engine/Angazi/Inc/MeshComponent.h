#pragma once

#include "Component.h"

namespace Angazi
{
	class TransformComponent;

	class MeshComponent : public Component
	{
	public:
		META_CLASS_DECLARE;

		void Initialize() override;
		void Render() override;

		void Terminate() override;

	private:
		const TransformComponent* mTransformComponent = nullptr;
		std::string mMeshFileName;
		Graphics::MeshId mMeshId;

		Graphics::StandardEffect mStandardEffect;
		Graphics::DirectionalLight mDirectionalLight;
		Graphics::Material mMaterial;
	};
}