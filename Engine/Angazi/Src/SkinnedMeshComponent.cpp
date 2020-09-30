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
META_FIELD(mModelFileName, "ModelFileName")
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
	auto model = ModelManager::Get()->GetModel(mModelId);

	if (model)
	{
		animator.Initialize(*model);
		for (int n = 0; n < model->animationSet.clips.size(); n++)
			animator.SetClipLooping(n, true);
		animator.PlayAnimation(0);
	}
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

	SimpleDraw::Render(camera);
	shader->UseShadow(false);
	shader->SetSkinnedMesh(true);
	auto matWorld = mTransformComponent->GetTransform();
	shader->Begin();
	shader->SetMaterial(mMaterialComponent->material);
	shader->SetDirectionalLight(light);
	shader->SetTransformData(matWorld, camera.GetViewMatrix(), camera.GetPerspectiveMatrix(), camera.GetPosition());
	auto boneMat = animator.GetBoneMatrices();
	shader->SetBoneTransforms(boneMat);
	shader->UpdateSettings();

	if (showSkeleton)
		DrawSkeleton(model->skeleton, animator.GetBoneMatrices(), 3.0f);
	else
		model->Draw(shader);
	shader->End();
	SimpleDraw::Render(camera, matWorld);
}

void SkinnedMeshComponent::Update(float deltaTime)
{
	if (currentAnimation == -1)
		animator.ComputeBindPose();
	constexpr float dt = 1.0f / 60.0f;
	animator.Update(dt);
}

void SkinnedMeshComponent::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Skinned Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Columns(2, "SkinnedMeshRenderer");
		//ImGui::SetColumnWidth(0, 80.f);

		ImGui::Text("File Path");
		ImGui::NextColumn();
		ImGui::Text("%s", mModelFileName.u8string().c_str());
		ImGui::NextColumn();

		ImGui::Text("Change Model");
		ImGui::NextColumn();
		if (ImGui::Button("Change"))
			ChangeSkinnedMesh("Change Model", mModelId, mModelFileName);
		ImGui::NextColumn();

		auto model = ModelManager::Get()->GetModel(mModelId);

		ImGui::Text("Animation");
		ImGui::NextColumn();
		const char* combo_label = model->animationSet.clips[currentAnimation]->name.c_str();
		if (ImGui::BeginCombo("##Animations", combo_label))
		{
			for (int n = 0; n < model->animationSet.clips.size(); n++)
			{
				ImGui::PushID(&model->animationSet.clips[n]);
				const bool is_selected = (currentAnimation == n);
				if (ImGui::Selectable(model->animationSet.clips[n]->name.c_str(), is_selected))
				{
					currentAnimation = n;
					animator.BlendTo(currentAnimation, 0.5f);
				}
				ImGui::PopID();
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::NextColumn();

		ImGui::Columns(1);

		if (ImGui::Checkbox("Show Bones", &showSkeleton))
			animator.SetShowSkeleton(showSkeleton);

	}
}


