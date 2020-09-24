#include "Precompiled.h"
#include "SkinnedMeshComponent.h"

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

void SkinnedMeshComponent::Initialize()
{
	mModelId = ModelManager::Get()->LoadModel(mModelFileName);
	mTransformComponent = GetGameObject().GetComponent<TransformComponent>();
	mMaterialComponent = GetGameObject().GetComponent<MaterialComponent>();
}

void SkinnedMeshComponent::Render()
{
	auto model = ModelManager::Get()->GetModel(mModelId);
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

	}
}


