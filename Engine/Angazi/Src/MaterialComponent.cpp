#include "Precompiled.h"
#include "MaterialComponent.h"

using namespace Angazi;

#include "ImGui/Inc/imgui.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "CameraService.h"

using namespace Angazi;
using namespace Angazi::Graphics;

META_CLASS_BEGIN(Material)
	META_FIELD_BEGIN
		META_FIELD(ambient, "Ambient")
		META_FIELD(diffuse, "Diffuse")
		META_FIELD(specular, "Specular")
		META_FIELD(power, "Power")
	META_FIELD_END
META_CLASS_END;

META_DERIVED_BEGIN(MaterialComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(mDiffuseFilePath, "DiffuseFilePath")
		META_FIELD(mNormalFilePath, "NormalFilePath")
		META_FIELD(mSpecularFilePath, "SpecularFilePath")
		META_FIELD(mDisplacementFilePath, "DisplacementFilePath")
		META_FIELD(mAmbientOcculsionFilePath, "AmbientOcculsionFilePath")
		META_FIELD(mMetallicFilePath, "MetallicFilePath")
		META_FIELD(mRoughnessFilePath, "RoughnessFilePath")
		META_FIELD(material, "Material")
	META_FIELD_END
META_CLASS_END;

void MaterialComponent::Initialize()
{
	diffuseId = TextureManager::Get()->Load(mDiffuseFilePath, false, false);
	normalId = TextureManager::Get()->Load(mNormalFilePath, false, false);
	specularId = TextureManager::Get()->Load(mSpecularFilePath, false, false);
	displacementId = TextureManager::Get()->Load(mDisplacementFilePath, false, false);
	ambientOcculsionId = TextureManager::Get()->Load(mAmbientOcculsionFilePath, false, false);
	metallicId = TextureManager::Get()->Load(mMetallicFilePath, false, false);
	roughnessId = TextureManager::Get()->Load(mRoughnessFilePath, false, false);
}

void MaterialComponent::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Material", &material.ambient.x);
		ImGui::ColorEdit4("Diffuse##Material", &material.diffuse.x);
		ImGui::ColorEdit4("Specular##Material", &material.specular.x);
		ImGui::DragFloat("Power##Material", &material.power, 1.0f, 1.0f, 100.0f);

		auto textureManager = TextureManager::Get();
		constexpr float imageSize = 50.0f;
		ImGui::Image(textureManager->GetTexture(diffuseId)->GetShaderResourceView(), { imageSize,imageSize });
		ImGui::Image(textureManager->GetTexture(normalId)->GetShaderResourceView(), { imageSize,imageSize });
		ImGui::Image(textureManager->GetTexture(displacementId)->GetShaderResourceView(), { imageSize,imageSize });
		ImGui::Image(textureManager->GetTexture(ambientOcculsionId)->GetShaderResourceView(), { imageSize,imageSize });
		ImGui::Image(textureManager->GetTexture(specularId)->GetShaderResourceView(), { imageSize,imageSize });
		ImGui::Image(textureManager->GetTexture(roughnessId)->GetShaderResourceView(), { imageSize,imageSize });
		ImGui::Image(textureManager->GetTexture(metallicId)->GetShaderResourceView(), { imageSize,imageSize });
	}
}
