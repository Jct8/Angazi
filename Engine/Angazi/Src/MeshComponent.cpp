#include "Precompiled.h"
#include "MeshComponent.h"

using namespace Angazi;

#include "Angazi.h"

#include "CameraService.h"
#include "EnvironmentService.h"
#include "LightService.h"
#include "ShaderService.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;

META_DERIVED_BEGIN(MeshComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(mMeshFileName, "MeshFileName")
	META_FIELD_END
META_CLASS_END;

namespace
{
	void ChangeMesh(const char* title, MeshId& meshId, std::filesystem::path& originalPath)
	{
		char filePath[MAX_PATH]{};
		const char* filter = "Obj Files (*.obj)\0*.obj";
		if (MainApp().OpenFileDialog(filePath, title, filter))
		{
			std::string x = filePath;
			originalPath = "..\\..\\" + x.substr(x.find("\\Assets\\") + 1);
			meshId = MeshManager::Get()->LoadMesh(originalPath);
		}
	}
}

void MeshComponent::Initialize()
{
	if (mInitialized)
		return;
	mMeshId = MeshManager::Get()->LoadMesh(mMeshFileName);
	mTransformComponent = GetGameObject().GetComponent<TransformComponent>();
	mMaterialComponent = GetGameObject().GetComponent<MaterialComponent>();
	if (!mMaterialComponent)
		mMaterialComponent = GetGameObject().AddComponent<MaterialComponent>();
	mInitialized = true;
}

void MeshComponent::Render()
{
	auto meshBuffer = MeshManager::Get()->GetModel(mMeshId);
	if (!meshBuffer)
		return;

	const auto& camera = GetGameObject().GetWorld().GetService<CameraService>()->GetActiveCamera();
	const auto& light = GetGameObject().GetWorld().GetService<LightService>()->GetActiveLight();
	const auto& env = GetGameObject().GetWorld().GetService<EnvironmentService>()->GetActiveEnvironment();
	const auto& shader = GetGameObject().GetWorld().GetService<ShaderService>()->GetShader<PbrEffect>();

	auto matWorld = mTransformComponent->GetTransform();
	auto texManager = TextureManager::Get();

	shader->useIBL(true);
	shader->Begin();
	shader->SetMaterial(mMaterialComponent->material);
	shader->SetDirectionalLight(light);

	shader->SetDiffuseTexture(texManager->GetTexture(mMaterialComponent->diffuseId));
	shader->SetNormalTexture(texManager->GetTexture(mMaterialComponent->normalId));
	shader->SetDisplacementTexture(texManager->GetTexture(mMaterialComponent->displacementId));
	shader->SetAOTexture(texManager->GetTexture(mMaterialComponent->ambientOcculsionId));
	//shader->SetSpecularTexture(texManager->GetTexture(mMaterialComponent->specularId));
	shader->SetRoughnessTexture(texManager->GetTexture(mMaterialComponent->roughnessId));
	shader->SetMetallicTexture(texManager->GetTexture(mMaterialComponent->metallicId));
	shader->SetPreFilterMap(env.GetPrefilteredMap());
	shader->SetIrradianceMap(env.GetIrradianceMap());

	shader->SetTransformData(matWorld, camera.GetViewMatrix(), camera.GetPerspectiveMatrix(), camera.GetPosition());
	shader->UpdateSettings();
	meshBuffer->Draw();
	shader->End();
}

void MeshComponent::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Columns(2, "MeshRenderer");
		ImGui::SetColumnWidth(0, 80.f);

		ImGui::Text("File Path");
		ImGui::NextColumn();
		ImGui::Text("%s",mMeshFileName.u8string().c_str());
		ImGui::NextColumn();
		ImGui::Columns(1);
		if (ImGui::Button("Change Mesh"))
			ChangeMesh("Change Mesh",mMeshId,mMeshFileName);
	}
}

