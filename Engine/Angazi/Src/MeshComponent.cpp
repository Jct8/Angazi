#include "Precompiled.h"
#include "MeshComponent.h"

using namespace Angazi;

#include "TransformComponent.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "CameraService.h"

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

	mStandardEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mStandardEffect.UseShadow(false);
	mStandardEffect.SetSkinnedMesh(false);

	mDirectionalLight.direction = Math::Normalize({ 0.327f,-0.382f, 0.864f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

	mMaterial.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mMaterial.power = 80.0f;
}

void MeshComponent::Render()
{
	auto meshBuffer = MeshManager::Get()->GetModel(mMeshId);
	auto camera = GetGameObject().GetWorld().GetService<CameraService>()->GetActiveCamera();

	auto matWorld = mTransformComponent->GetTransform();
	mStandardEffect.Begin();
	mStandardEffect.SetMaterial(mMaterial);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetTransformData(matWorld, camera.GetViewMatrix(), camera.GetPerspectiveMatrix(), camera.GetPosition());
	mStandardEffect.UpdateSettings();
	meshBuffer->Draw();
	mStandardEffect.End();
}

void MeshComponent::Terminate()
{
	mStandardEffect.Terminate();
}


