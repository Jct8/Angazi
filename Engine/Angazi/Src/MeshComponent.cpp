#include "Precompiled.h"
#include "MeshComponent.h"

using namespace Angazi;


#include "CameraService.h"
#include "EnvironmentService.h"
#include "LightService.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"

using namespace Angazi;
using namespace Angazi::Graphics;

META_DERIVED_BEGIN(MeshComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(mMeshFileName, "MeshFileName")
	META_FIELD_END
META_CLASS_END;

void MeshComponent::Initialize()
{
	mMeshId = MeshManager::Get()->LoadMesh(mMeshFileName);
	mTransformComponent = GetGameObject().GetComponent<TransformComponent>();
	mMaterialComponent = GetGameObject().GetComponent<MaterialComponent>();

	mStandardEffect.Initialize();
}

void MeshComponent::Render()
{
	auto meshBuffer = MeshManager::Get()->GetModel(mMeshId);
	const auto& camera = GetGameObject().GetWorld().GetService<CameraService>()->GetActiveCamera();
	const auto& light = GetGameObject().GetWorld().GetService<LightService>()->GetActiveLight();
	auto& env = GetGameObject().GetWorld().GetService<EnvironmentService>()->GetActiveEnvironment();

	auto matWorld = mTransformComponent->GetTransform();
	auto texManager = TextureManager::Get();

	mStandardEffect.Begin();
	mStandardEffect.SetMaterial(mMaterialComponent->material);
	mStandardEffect.SetDirectionalLight(light);

	mStandardEffect.SetDiffuseTexture(texManager->GetTexture(mMaterialComponent->diffuseId));
	mStandardEffect.SetNormalTexture(texManager->GetTexture(mMaterialComponent->normalId));
	mStandardEffect.SetDisplacementTexture(texManager->GetTexture(mMaterialComponent->displacementId));
	mStandardEffect.SetAOTexture(texManager->GetTexture(mMaterialComponent->ambientOcculsionId));
	mStandardEffect.SetSpecularTexture(texManager->GetTexture(mMaterialComponent->specularId));
	//mStandardEffect.SetRoughnessTexture(texManager->GetTexture(mMaterialComponent->roughnessId));
	//mStandardEffect.SetMetallicTexture(texManager->GetTexture(mMaterialComponent->metallicId));
	//mStandardEffect.SetPreFilterMap(env.GetPrefilteredMap());
	//mStandardEffect.SetIrradianceMap(env.GetIrradianceMap());

	mStandardEffect.SetTransformData(matWorld, camera.GetViewMatrix(), camera.GetPerspectiveMatrix(), camera.GetPosition());
	mStandardEffect.UpdateSettings();
	meshBuffer->Draw();
	mStandardEffect.End();
}

void MeshComponent::Terminate()
{
	mStandardEffect.Terminate();
}


