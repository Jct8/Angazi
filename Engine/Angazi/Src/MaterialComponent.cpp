#include "Precompiled.h"
#include "MaterialComponent.h"

#include "Angazi.h"
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

namespace
{
	void ChangeTextureId(const char* title, TextureId& textureId, std::filesystem::path& originalPath)
	{
		char filePath[MAX_PATH]{};
		const char* filter = "PNG Files (*.png)\0*.png;\0JPG Files (*.jpg)\0*.jpg";
		if (MainApp().OpenFileDialog(filePath, title, filter))
		{
			if (std::filesystem::exists(filePath))
			{
				std::string x = filePath;
				originalPath = "..\\..\\" + x.substr(x.find("\\Assets\\") + 1);
				textureId = TextureManager::Get()->Load(originalPath);
			}
		}
	}
}

void MaterialComponent::Initialize()
{
	if (mInitialized)
		return;
	diffuseId = TextureManager::Get()->Load(mDiffuseFilePath, true, false);
	normalId = TextureManager::Get()->Load(mNormalFilePath, false, false);
	specularId = TextureManager::Get()->Load(mSpecularFilePath, false, false);
	displacementId = TextureManager::Get()->Load(mDisplacementFilePath, false, false);
	ambientOcculsionId = TextureManager::Get()->Load(mAmbientOcculsionFilePath, false, false);
	metallicId = TextureManager::Get()->Load(mMetallicFilePath, false, false);
	roughnessId = TextureManager::Get()->Load(mRoughnessFilePath, false, false);
	mInitialized = true;
}

void MaterialComponent::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Columns(2,"Materials");

		ImGui::Text("Ambient Color");
		ImGui::NextColumn();
		ImGui::ColorEdit4("##MaterialAmbient", &material.ambient.x, ImGuiColorEditFlags_NoInputs);
		ImGui::NextColumn();

		ImGui::Text("Diffuse Color");
		ImGui::NextColumn();
		ImGui::ColorEdit4("##MaterialDiffuse", &material.diffuse.x, ImGuiColorEditFlags_NoInputs);
		ImGui::NextColumn();

		ImGui::Text("Specular Color");
		ImGui::NextColumn();
		ImGui::ColorEdit4("##MaterialSpecular", &material.specular.x, ImGuiColorEditFlags_NoInputs);
		ImGui::NextColumn();

		ImGui::Text("Shininess");
		ImGui::NextColumn();
		ImGui::DragFloat("##MaterialShininess", &material.power, 1.0f, 1.0f, 100.0f);
		ImGui::NextColumn();

		ImGui::Columns(1);

		auto textureManager = TextureManager::Get();
		constexpr float imageSize = 30.0f;
		if (ImGui::TreeNode("Textures"))
		{
			ImGui::NewLine();
			ImGui::Columns(2, "Textures");

			ImGui::PushID("Albedo");
			ImGui::Text("Albedo");
			ImGui::NextColumn();
			if (ImGui::ImageButton(textureManager->GetTexture(diffuseId)->GetShaderResourceView(), { imageSize, imageSize }, {}, {1,1}, 0))
				ChangeTextureId("Change Albedo Texture",diffuseId,mDiffuseFilePath);
			if (ImGui::IsItemHovered()) 
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			ImGui::NextColumn();
			ImGui::PopID();

			ImGui::PushID("Normal");
			ImGui::Text("Normal Map");
			ImGui::NextColumn();
			if(ImGui::ImageButton(textureManager->GetTexture(normalId)->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
				ChangeTextureId("Change Normal Texture", normalId, mNormalFilePath);
			if (ImGui::IsItemHovered()) 
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			ImGui::NextColumn();
			ImGui::PopID();

			ImGui::PushID("Displacement");
			ImGui::Text("Displacement Map");
			ImGui::NextColumn();
			if (ImGui::ImageButton(textureManager->GetTexture(displacementId)->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
				ChangeTextureId("Change Displacement Texture", displacementId, mDisplacementFilePath);
			if (ImGui::IsItemHovered()) 
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			ImGui::NextColumn();
			ImGui::PopID();

			ImGui::PushID("Ambient Occulsion");
			ImGui::Text("Ambient Occulsion");
			ImGui::NextColumn();
			if (ImGui::ImageButton(textureManager->GetTexture(ambientOcculsionId)->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
				ChangeTextureId("Change Ambient Occulsion Texture", ambientOcculsionId, mAmbientOcculsionFilePath);
			if (ImGui::IsItemHovered())
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			ImGui::NextColumn();
			ImGui::PopID();

			ImGui::PushID("Specular Map");
			ImGui::Text("Specular Map");
			ImGui::NextColumn();
			if (ImGui::ImageButton(textureManager->GetTexture(specularId)->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
				ChangeTextureId("Change Specular Texture", specularId, mSpecularFilePath);
			if (ImGui::IsItemHovered()) 
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			ImGui::NextColumn();
			ImGui::PopID();

			ImGui::PushID("Roughness");
			ImGui::Text("Roughness");
			ImGui::NextColumn();
			if (ImGui::ImageButton(textureManager->GetTexture(roughnessId)->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
				ChangeTextureId("Change Roughness Texture", roughnessId, mRoughnessFilePath);
			if (ImGui::IsItemHovered()) 
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			ImGui::NextColumn();
			ImGui::PopID();

			ImGui::PushID("Metallic");
			ImGui::Text("Metallic");
			ImGui::NextColumn();
			if (ImGui::ImageButton(textureManager->GetTexture(metallicId)->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
				ChangeTextureId("Change Metallic Texture", metallicId, mMetallicFilePath);
			if (ImGui::IsItemHovered()) 
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			ImGui::NextColumn();
			ImGui::PopID();

			ImGui::Columns(1);
			ImGui::TreePop();
		}
	}
}
