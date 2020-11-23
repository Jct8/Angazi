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
		const char* filter = "Supported Textures(*.png, *.jpg)\0*.png;*.jpg";
		//const char* filter = "PNG Files (*.png)\0*.png;\0JPG Files (*.jpg)\0*.jpg";
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
	diffuseId.emplace_back(TextureManager::Get()->Load(mDiffuseFilePath, true, false));
	normalId.emplace_back(TextureManager::Get()->Load(mNormalFilePath, false, false));
	specularId.emplace_back(TextureManager::Get()->Load(mSpecularFilePath, false, false));
	displacementId.emplace_back(TextureManager::Get()->Load(mDisplacementFilePath, false, false));
	ambientOcculsionId.emplace_back(TextureManager::Get()->Load(mAmbientOcculsionFilePath, false, false));
	metallicId.emplace_back(TextureManager::Get()->Load(mMetallicFilePath, false, false));
	roughnessId.emplace_back(TextureManager::Get()->Load(mRoughnessFilePath, false, false));
	material.ambient = { 1.0f };
	material.diffuse = { 1.0f };
	material.specular = { 0.5f,0.5f,0.5f,1.0f };
	mInitialized = true;
}

void MaterialComponent::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Columns(2, "Materials");

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
		for (size_t i = 0; i < diffuseId.size(); i++)
		{
			if (ImGui::TreeNode("Material" + i ))
			{
				auto textureManager = TextureManager::Get();
				constexpr float imageSize = 30.0f;
				if (ImGui::TreeNode("Textures"))
				{
					ImGui::NewLine();
					ImGui::Columns(2, "Textures");

					ImGui::PushID("Albedo");
					ImGui::Text("Albedo");
					ImGui::NextColumn();
					if (ImGui::ImageButton(textureManager->GetTexture(diffuseId[i])->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
						ChangeTextureId("Change Albedo Texture", diffuseId[i], mDiffuseFilePath);
					if (ImGui::IsItemHovered())
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					ImGui::NextColumn();
					ImGui::PopID();

					ImGui::PushID("Normal");
					ImGui::Text("Normal Map");
					ImGui::NextColumn();
					if (ImGui::ImageButton(textureManager->GetTexture(normalId[i])->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
						ChangeTextureId("Change Normal Texture", normalId[i], mNormalFilePath);
					if (ImGui::IsItemHovered())
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					ImGui::NextColumn();
					ImGui::PopID();

					ImGui::PushID("Displacement");
					ImGui::Text("Displacement Map");
					ImGui::NextColumn();
					if (ImGui::ImageButton(textureManager->GetTexture(displacementId[i])->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
						ChangeTextureId("Change Displacement Texture", displacementId[i], mDisplacementFilePath);
					if (ImGui::IsItemHovered())
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					ImGui::NextColumn();
					ImGui::PopID();

					ImGui::PushID("Ambient Occulsion");
					ImGui::Text("Ambient Occulsion");
					ImGui::NextColumn();
					if (ImGui::ImageButton(textureManager->GetTexture(ambientOcculsionId[i])->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
						ChangeTextureId("Change Ambient Occulsion Texture", ambientOcculsionId[i], mAmbientOcculsionFilePath);
					if (ImGui::IsItemHovered())
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					ImGui::NextColumn();
					ImGui::PopID();

					ImGui::PushID("Specular Map");
					ImGui::Text("Specular Map");
					ImGui::NextColumn();
					if (ImGui::ImageButton(textureManager->GetTexture(specularId[i])->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
						ChangeTextureId("Change Specular Texture", specularId[i], mSpecularFilePath);
					if (ImGui::IsItemHovered())
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					ImGui::NextColumn();
					ImGui::PopID();

					ImGui::PushID("Roughness");
					ImGui::Text("Roughness");
					ImGui::NextColumn();
					if (ImGui::ImageButton(textureManager->GetTexture(roughnessId[i])->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
						ChangeTextureId("Change Roughness Texture", roughnessId[i], mRoughnessFilePath);
					if (ImGui::IsItemHovered())
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					ImGui::NextColumn();
					ImGui::PopID();

					ImGui::PushID("Metallic");
					ImGui::Text("Metallic");
					ImGui::NextColumn();
					if (ImGui::ImageButton(textureManager->GetTexture(metallicId[i])->GetShaderResourceView(), { imageSize, imageSize }, {}, { 1,1 }, 0))
						ChangeTextureId("Change Metallic Texture", metallicId[i], mMetallicFilePath);
					if (ImGui::IsItemHovered())
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					ImGui::NextColumn();
					ImGui::PopID();

					ImGui::Columns(1);
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}
	}
}
