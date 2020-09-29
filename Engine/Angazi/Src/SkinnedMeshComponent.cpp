#include "Precompiled.h"
#include "SkinnedMeshComponent.h"

#include "Angazi.h"

#include "GameObject.h"
#include "GameWorld.h"

#include "CameraService.h"
#include "LightService.h"
#include "ShaderService.h"

#include "ImGui/Inc/imgui.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"

using namespace Angazi;
using namespace Angazi::Graphics;

META_DERIVED_BEGIN(SkinnedMeshComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(mModelFileName,"ModelFileName")
	META_FIELD_END
META_CLASS_END;

namespace
{
	void ChangeSkinnedMesh(const char* title, ModelId& modelId, std::filesystem::path& originalPath)
	{
		char filePath[MAX_PATH]{};
		const char* filter = "Model Files (*.model)\0*.model";
		if (MainApp().OpenFileDialog(filePath, title, filter))
		{
			std::string x = filePath;
			originalPath = "..\\..\\" + x.substr(x.find("\\Assets\\") + 1);
			modelId = ModelManager::Get()->LoadModel(originalPath);
		}
	}
}

void SkinnedMeshComponent::Initialize()
{
	if (mInitialized)
		return;
	mModelId = ModelManager::Get()->LoadModel(mModelFileName);
	mTransformComponent = GetGameObject().GetComponent<TransformComponent>();
	mMaterialComponent = GetGameObject().GetComponent<MaterialComponent>();
	if (!mMaterialComponent)
		mMaterialComponent = GetGameObject().AddComponent<MaterialComponent>();
	mInitialized = true;
}

void SkinnedMeshComponent::Render()
{
	auto model = ModelManager::Get()->GetModel(mModelId);
	if (!model)
		return;

	const auto& camera = GetGameObject().GetWorld().GetService<CameraService>()->GetActiveCamera();
	const auto& light = GetGameObject().GetWorld().GetService<LightService>()->GetActiveLight();
	const auto& shader = GetGameObject().GetWorld().GetService<ShaderService>()->GetShader<StandardEffect>();

	shader->UseShadow(false);
	shader->SetSkinnedMesh(false);
	auto matWorld = mTransformComponent->GetTransform();
	shader->Begin();
	shader->SetMaterial(mMaterialComponent->material);
	shader->SetDirectionalLight(light);
	shader->SetTransformData(matWorld, camera.GetViewMatrix(), camera.GetPerspectiveMatrix(), camera.GetPosition());
	shader->UpdateSettings();
	model->Draw(shader);
	shader->End();
}

void SkinnedMeshComponent::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Skinned Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Columns(2, "SkinnedMeshRenderer");
		ImGui::SetColumnWidth(0, 80.f);

		ImGui::Text("File Path");
		ImGui::NextColumn();
		ImGui::Text("%s", mModelFileName.u8string().c_str());
		ImGui::NextColumn();
		ImGui::Columns(1);
		if (ImGui::Button("Change Model"))
			ChangeSkinnedMesh("Change Model", mModelId, mModelFileName);
	}
}


