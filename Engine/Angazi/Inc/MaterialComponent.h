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

		std::vector<Graphics::TextureId> diffuseId;
		std::vector<Graphics::TextureId> normalId;
		std::vector<Graphics::TextureId> specularId;
		std::vector<Graphics::TextureId> displacementId;
		std::vector<Graphics::TextureId> ambientOcculsionId;
		std::vector<Graphics::TextureId> metallicId;
		std::vector<Graphics::TextureId> roughnessId;
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