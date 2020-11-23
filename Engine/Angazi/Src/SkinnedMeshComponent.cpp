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
		META_FIELD(mIsCastingShadow, "CastingShadow")
		META_FIELD(mIsReceivingShadows, "ReceivingShadows")
		META_FIELD(mShowSkeleton, "ShowSkeleton")
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
	{
		mMaterialComponent = GetGameObject().AddComponent<MaterialComponent>();
		mMaterialComponent->Initialize();
	}
	
	InitializeAnimator();

	mInitialized = true;
}

void SkinnedMeshComponent::Terminate()
{
	animator.Terminate();
}

void SkinnedMeshComponent::Render()
{
	auto model = ModelManager::Get()->GetModel(mModelId);
	if (!model)
		return;

	const auto& camera = GetGameObject().GetWorld().GetService<CameraService>()->GetActiveCamera();
	const auto& light = GetGameObject().GetWorld().GetService<LightService>()->GetActiveLight();
	const auto& shader = GetGameObject().GetWorld().GetService<ShaderService>()->GetShader<StandardEffect>();
	const auto& shadowShader = GetGameObject().GetWorld().GetService<ShaderService>()->GetShader<ShadowEffect>();

	SimpleDraw::Render(camera);

	auto matWorld = mTransformComponent->GetTransform();
	auto boneMat = animator.GetBoneMatrices();

	auto lightVP = shadowShader->GetVPMatrix();
	auto wvpLight = Transpose(matWorld * lightVP);

	shader->UpdateShadowBuffer(wvpLight);
	shader->SetSkinnedMesh(true);
	shader->SetDepthTexture(shadowShader->GetRenderTarget());
	shader->UseShadow(mIsReceivingShadows);
	shader->Begin();
	shader->SetMaterial(mMaterialComponent->material);
	shader->SetDirectionalLight(light);
	shader->SetTransformData(matWorld, camera.GetViewMatrix(), camera.GetPerspectiveMatrix(), camera.GetPosition());
	shader->SetBoneTransforms(boneMat);
	shader->UpdateSettings();

	if (mShowSkeleton)
		DrawSkeleton(model->skeleton, animator.GetBoneMatrices(), 3.0f);
	else
		model->Draw(shader);
	shader->End();
	SimpleDraw::Render(camera, matWorld);
}

void SkinnedMeshComponent::RenderShadow()
{
	if (mIsCastingShadow && !mShowSkeleton)
	{
		const auto& shadowEffect = GetGameObject().GetWorld().GetService<ShaderService>()->GetShader<ShadowEffect>();
		const auto& matWorld = mTransformComponent->GetTransform();
		const auto& model = ModelManager::Get()->GetModel(mModelId);
		if (!model)
			return;

		shadowEffect->SetWorldMatrix(matWorld);
		shadowEffect->SetBoneTransforms(animator.GetBoneMatrices());
		shadowEffect->SetSkinnedMesh(true);
		shadowEffect->UpdateSettings();
		model->Draw(shadowEffect);
	}
}

void SkinnedMeshComponent::Update(float deltaTime)
{
	if (currentAnimation == -1)
		animator.ComputeBindPose();
	constexpr float dt = 1.0f / 60.0f;
	if(isPlayingAnimation)
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
		{
			ChangeSkinnedMesh("Change Model", mModelId, mModelFileName);
			InitializeAnimator();
		}
		ImGui::NextColumn();

		auto model = ModelManager::Get()->GetModel(mModelId);

		ImGui::Text("Animations");
		ImGui::NextColumn();
		const char* combo_label = model != nullptr ? model->animationSet.clips[currentAnimation]->name.c_str() : "No Model";
		if (ImGui::BeginCombo("##Animations", combo_label))
		{
			if (model)
			{
				for (size_t n = 0; n < model->animationSet.clips.size(); n++)
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
			}
			ImGui::EndCombo();
		}
		ImGui::NextColumn();

		ImGui::Text("Play Animation"); ImGui::SameLine();
		ImGui::NextColumn();
		ImGui::Checkbox("##StopPlayAnimation", &isPlayingAnimation);
		ImGui::NextColumn();

		ImGui::Text("Cast Shadows"); ImGui::SameLine();
		ImGui::NextColumn();
		ImGui::Checkbox("##CastShadow", &mIsCastingShadow);
		ImGui::NextColumn();

		ImGui::Text("Receive Shadows"); ImGui::SameLine();
		ImGui::NextColumn();
		ImGui::Checkbox("##ReceiveShadow", &mIsReceivingShadows);
		ImGui::NextColumn();

		ImGui::Text("Show Bones"); ImGui::SameLine();
		ImGui::NextColumn();
		if (ImGui::Checkbox("##ShowBones", &mShowSkeleton))
			animator.SetShowSkeleton(mShowSkeleton);
		ImGui::NextColumn();

		ImGui::Columns(1);
	}
}

void SkinnedMeshComponent::InitializeAnimator()
{
	auto model = ModelManager::Get()->GetModel(mModelId);
	if (model)
	{
		animator.Initialize(*model);
		for (size_t n = 0; n < model->animationSet.clips.size(); n++)
			animator.SetClipLooping(n, true);
		animator.PlayAnimation(0);
	}
}


