#pragma once

#include "Component.h"

namespace Angazi
{
	class TransformComponent;

	class MaterialComponent : public Component
	{
	public:
		META_CLASS_DECLARE;
		
		void Initialize() override;
		void ShowInspectorProperties() override;

		Graphics::TextureId diffuseId = 0;
		Graphics::TextureId normalId = 0;
		Graphics::TextureId specularId = 0;
		Graphics::TextureId displacementId = 0;
		Graphics::TextureId ambientOcculsionId = 0;
		Graphics::TextureId metallicId = 0;
		Graphics::TextureId roughnessId = 0;
		Graphics::Material material;

	private:
		std::filesystem::path mDiffuseFilePath;
		std::filesystem::path mNormalFilePath;
		std::filesystem::path mSpecularFilePath;
		std::filesystem::path mDisplacementFilePath;
		std::filesystem::path mAmbientOcculsionFilePath;
		std::filesystem::path mMetallicFilePath;
		std::filesystem::path mRoughnessFilePath;
	};
}