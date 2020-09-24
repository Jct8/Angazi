#include "Precompiled.h"
#include "SkinnedMeshComponent.h"

#include "GameObject.h"
#include "GameWorld.h"

#include "CameraService.h"
#include "LightService.h"

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

	mModelStandardEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mModelStandardEffect.UseShadow(false);
	mModelStandardEffect.SetNormalMapWeight(1.0f);
	mModelStandardEffect.SetSkinnedMesh(false);
}

void SkinnedMeshComponent::Render()
{
	auto model = ModelManager::Get()->GetModel(mModelId);
	const auto& camera = GetGameObject().GetWorld().GetService<CameraService>()->GetActiveCamera();
	const auto& light = GetGameObject().GetWorld().GetService<LightService>()->GetActiveLight();

	auto matWorld = mTransformComponent->GetTransform();
	mModelStandardEffect.Begin();
	mModelStandardEffect.SetMaterial(mMaterialComponent->material);
	mModelStandardEffect.SetDirectionalLight(light);
	mModelStandardEffect.SetTransformData(matWorld, camera.GetViewMatrix(), camera.GetPerspectiveMatrix(), camera.GetPosition());
	mModelStandardEffect.UpdateSettings();
	model->Draw(&mModelStandardEffect);
	mModelStandardEffect.End();
}

void SkinnedMeshComponent::Terminate()
{
	mModelStandardEffect.Terminate();
}

void SkinnedMeshComponent::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Skinned Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}


