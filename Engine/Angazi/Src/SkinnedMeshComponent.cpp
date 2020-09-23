#include "Precompiled.h"
#include "SkinnedMeshComponent.h"

#include "CameraService.h"
#include "GameObject.h"
#include "GameWorld.h"
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

	mDirectionalLight.direction = Math::Normalize({ 0.327f,-0.382f, 0.864f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

}

void SkinnedMeshComponent::Render()
{
	auto model = ModelManager::Get()->GetModel(mModelId);
	auto camera = GetGameObject().GetWorld().GetService<CameraService>()->GetActiveCamera();
	
	auto matWorld = mTransformComponent->GetTransform();
	mModelStandardEffect.Begin();
	mModelStandardEffect.SetMaterial(mMaterialComponent->material);
	mModelStandardEffect.SetDirectionalLight(mDirectionalLight);
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


