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
		META_FIELD(mIsCastingShadow, "CastingShadow")
		META_FIELD(mIsReceivingShadows, "ReceivingShadows")
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
	const auto& shader = GetGameObject().GetWorld().GetService<ShaderService>()->GetShader<StandardEffect>();
	const auto& pbrShader = GetGameObject().GetWorld().GetService<ShaderService>()->GetShader<PbrEffect>();
	const auto& shadowShader = GetGameObject().GetWorld().GetService<ShaderService>()->GetShader<ShadowEffect>();

	auto matWorld = mTransformComponent->GetTransform();
	auto texManager = TextureManager::Get();
	auto lightVP = shadowShader->GetVPMatrix();
	auto wvpLight = Transpose(matWorld * lightVP);

	if (mMaterialComponent->roughnessId != 0 && mMaterialComponent->metallicId != 0)
	{
		pbrShader->Begin();

		pbrShader->SetMaterial(mMaterialComponent->material);
		pbrShader->SetDirectionalLight(light);
		pbrShader->SetDepthTexture(shadowShader->GetRenderTarget());

		pbrShader->SetDiffuseTexture(texManager->GetTexture(mMaterialComponent->diffuseId));
		if(mMaterialComponent->normalId != 0)
			pbrShader->SetNormalTexture(texManager->GetTexture(mMaterialComponent->normalId));
		if (mMaterialComponent->displacementId != 0)
			pbrShader->SetDisplacementTexture(texManager->GetTexture(mMaterialComponent->displacementId));
		if (mMaterialComponent->ambientOcculsionId != 0)
			pbrShader->SetAOTexture(texManager->GetTexture(mMaterialComponent->ambientOcculsionId));
		if (mMaterialComponent->roughnessId != 0)
			pbrShader->SetRoughnessTexture(texManager->GetTexture(mMaterialComponent->roughnessId));
		if (mMaterialComponent->metallicId != 0)
			pbrShader->SetMetallicTexture(texManager->GetTexture(mMaterialComponent->metallicId));
		pbrShader->SetPreFilterMap(env.GetPrefilteredMap());
		pbrShader->SetIrradianceMap(env.GetIrradianceMap());

		pbrShader->useIBL(true);
		pbrShader->UseShadow(mIsReceivingShadows);
		pbrShader->SetSkinnedMesh(false);
		pbrShader->SetTransformData(matWorld, camera.GetViewMatrix(), camera.GetPerspectiveMatrix(), camera.GetPosition());
		pbrShader->UpdateShadowBuffer(wvpLight);
		pbrShader->UpdateSettings();

		meshBuffer->Draw();
		pbrShader->End();
	}
	else
	{
		shader->Begin();

		shader->SetMaterial(mMaterialComponent->material);
		shader->SetDirectionalLight(light);
		shader->SetDepthTexture(shadowShader->GetRenderTarget());

		shader->SetDiffuseTexture(texManager->GetTexture(mMaterialComponent->diffuseId));
		if (mMaterialComponent->displacementId != 0)
			shader->SetDisplacementTexture(texManager->GetTexture(mMaterialComponent->displacementId));
		if (mMaterialComponent->ambientOcculsionId != 0)
			shader->SetAOTexture(texManager->GetTexture(mMaterialComponent->ambientOcculsionId));
		if (mMaterialComponent->specularId != 0)
			shader->SetSpecularTexture(texManager->GetTexture(mMaterialComponent->specularId));

		shader->SetSkinnedMesh(false);
		shader->UseShadow(mIsReceivingShadows);
		shader->SetTransformData(matWorld, camera.GetViewMatrix(), camera.GetPerspectiveMatrix(), camera.GetPosition());
		shader->UpdateShadowBuffer(wvpLight);
		shader->UpdateSettings();

		meshBuffer->Draw();
		shader->End();
	}
}

void MeshComponent::RenderShadow()
{
	if (mIsCastingShadow)
	{
		const auto& shadowEffect = GetGameObject().GetWorld().GetService<ShaderService>()->GetShader<ShadowEffect>();
		const auto& matWorld = mTransformComponent->GetTransform();
		const auto& meshBuffer = MeshManager::Get()->GetModel(mMeshId);
		if (!meshBuffer)
			return;

		shadowEffect->SetWorldMatrix(matWorld);
		shadowEffect->SetSkinnedMesh(false);
		shadowEffect->UpdateSettings();
		meshBuffer->Draw();
	}
}

void MeshComponent::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Columns(2, "MeshRenderer");

		ImGui::Text("File Path"); ImGui::SameLine();
		ImGui::NextColumn();
		ImGui::Text("%s", mMeshFileName.u8string().c_str());
		ImGui::NextColumn();

		ImGui::Text("Change Mesh"); ImGui::SameLine();
		ImGui::NextColumn();
		if (ImGui::Button("Change"))
			ChangeMesh("Change Mesh", mMeshId, mMeshFileName);
		ImGui::NextColumn();

		ImGui::Text("Cast Shadows"); ImGui::SameLine();
		ImGui::NextColumn();
		ImGui::Checkbox("##CastShadow", &mIsCastingShadow);
		ImGui::NextColumn();

		ImGui::Text("Recieve Shadows"); ImGui::SameLine();
		ImGui::NextColumn();
		ImGui::Checkbox("##RecieveShadow", &mIsReceivingShadows);
		ImGui::NextColumn();
		ImGui::Columns(1);
	}
}

