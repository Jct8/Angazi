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
		META_FIELD(specular,"Specular")
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
	diffuseId = TextureManager::Get()->Load(mDiffuseFilePath);
	normalId = TextureManager::Get()->Load(mNormalFilePath);
	specularId = TextureManager::Get()->Load(mSpecularFilePath);
	displacementId = TextureManager::Get()->Load(mDisplacementFilePath);
	ambientOcculsionId = TextureManager::Get()->Load(mAmbientOcculsionFilePath);
	metallicId = TextureManager::Get()->Load(mMetallicFilePath);
	roughnessId = TextureManager::Get()->Load(mRoughnessFilePath);
}

void MaterialComponent::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Material", &material.ambient.x);
		ImGui::ColorEdit4("Diffuse##Material", &material.diffuse.x);
		ImGui::ColorEdit4("Specular##Material", &material.specular.x);
		ImGui::DragFloat("Power##Material", &material.power, 1.0f, 1.0f, 100.0f);
	}
}
