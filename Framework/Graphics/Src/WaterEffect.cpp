#include "Precompiled.h"
#include "WaterEffect.h"

#include "LightTypes.h"
#include "Material.h"
#include "VertexTypes.h"
#include "Camera.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	Math::Vector3 cameraPosition;
	Math::Vector3 cameradirection;
}

void WaterEffect::Initialize(const std::filesystem::path & fileName)
{
	// Constant Buffers
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	// Shaders
	mVertexShader.Initialize(fileName, Vertex::Format);
	mPixelShader.Initialize(fileName);

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);

	// Textures
	SetDiffuseTexture("../../Assets/Images/Water/water2.jpg");
	SetSpecularTexture("../../Assets/Images/Water/waterSpec.jpg");
	SetDisplacementTexture("../../Assets/Images/Water/waterdudv.png");
	SetNormalTexture("../../Assets/Images/Water/waterNormal.png");

	auto graphicsSystem = GraphicsSystem::Get();
	mReflectionRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	mRefractionRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
}

void WaterEffect::Terminate()
{
	// Render targets
	mReflectionRenderTarget.Terminate();
	mRefractionRenderTarget.Terminate();

	// Textures
	mNormalMap.Terminate();
	mDisplacementMap.Terminate();
	mSpecularMap.Terminate();
	mDiffuseMap.Terminate();

	mSampler.Terminate();

	// Shaders
	mPixelShader.Terminate();
	mVertexShader.Terminate();

	// Constant Buffers
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void WaterEffect::Update(float deltaTime)
{
	mSettings.movement += mSettings.movementSpeed * deltaTime;
	if (mSettings.movement >= 1.0f)
		mSettings.movement -= 1.0f;
}

void WaterEffect::BeginRefraction()
{
	mRefractionRenderTarget.BeginRender();
}

void WaterEffect::EndRefraction()
{
	mRefractionRenderTarget.EndRender();
}

void WaterEffect::BeginReflection(Camera & camera, float waterHeight)
{
	cameraPosition = camera.GetPosition();
	cameradirection = camera.GetDirection();
	float distance = 2.0f * (cameraPosition.y - waterHeight);
	camera.SetPosition({ cameraPosition.x,cameraPosition.y - distance,cameraPosition.z });
	camera.SetDirection({ cameradirection.x,-cameradirection.y,cameradirection.z });

	mReflectionRenderTarget.BeginRender();
}

void WaterEffect::EndReflection(Camera & camera)
{
	mReflectionRenderTarget.EndRender();
	camera.SetPosition(cameraPosition);
	camera.SetDirection(cameradirection);
}

void WaterEffect::Begin()
{
	// Sampler
	mSampler.BindVS();
	mSampler.BindPS();

	// Constant Buffers
	mTransformBuffer.BindVS(0);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	// Maps
	mDiffuseMap.BindPS(0);
	mSpecularMap.BindPS(1);
	mDisplacementMap.BindVS(2);
	mDisplacementMap.BindPS(2);
	mNormalMap.BindPS(3);
	mRefractionRenderTarget.BindPS(4);
	mReflectionRenderTarget.BindPS(5);

	mVertexShader.Bind();
	mPixelShader.Bind();
}

void WaterEffect::End()
{
	// Constant Buffer
	mTransformBuffer.UnbindVS(0);
	mLightBuffer.UnbindVS(1);
	mLightBuffer.UnbindPS(1);
	mMaterialBuffer.UnbindVS(2);
	mMaterialBuffer.UnbindPS(2);
	mSettingsBuffer.UnbindVS(3);
	mSettingsBuffer.UnbindPS(3);
	mRefractionRenderTarget.UnbindPS(4);
	mReflectionRenderTarget.UnbindPS(5);

	// Textures
	mDiffuseMap.UnbindPS(0);
	mSpecularMap.UnbindPS(1);
	mDisplacementMap.UnbindVS(2);
	mDisplacementMap.UnbindPS(2);
	mNormalMap.UnbindPS(3);
	mRefractionRenderTarget.UnbindPS(4);
	mReflectionRenderTarget.UnbindPS(5);
}

void WaterEffect::SetWorldMatrix(const Math::Matrix4 & world)
{
	transformData.world = Math::Transpose(world);
	mTransformBuffer.Update(&transformData);
}

void WaterEffect::SetWVPMatrix(const Math::Matrix4 & world, const Math::Matrix4 & view, const Math::Matrix4 & projection)
{
	transformData.wvp = Math::Transpose(world * view * projection);
	mTransformBuffer.Update(&transformData);
}

void WaterEffect::SetViewProjection(const Math::Vector3 & viewProjection)
{
	transformData.viewPosition = viewProjection;
	mTransformBuffer.Update(&transformData);
}

void WaterEffect::SetDirectionalLight(const DirectionalLight & light)
{
	mLightBuffer.Update(&light);
}

void WaterEffect::SetMaterial(const Material & material)
{
	mMaterialBuffer.Update(&material);
}

void WaterEffect::SetDiffuseTexture(const std::filesystem::path & fileName)
{
	mDiffuseMap.Terminate();
	mDiffuseMap.Initialize(fileName);
}
void WaterEffect::SetSpecularTexture(const std::filesystem::path & fileName)
{
	mSettings.specularMapWeight = 1.0f;
	mSpecularMap.Terminate();
	mSpecularMap.Initialize(fileName);
}
void WaterEffect::SetDisplacementTexture(const std::filesystem::path & fileName)
{
	mSettings.bumpMapWeight = 1.0f;
	mDisplacementMap.Terminate();
	mDisplacementMap.Initialize(fileName);
}
void WaterEffect::SetNormalTexture(const std::filesystem::path & fileName)
{
	mSettings.normalMapWeight = 1.0f;
	mNormalMap.Terminate();
	mNormalMap.Initialize(fileName);
}

void WaterEffect::SetDiffuseTexture(const Texture * diffuseTexture)
{
	diffuseTexture->BindPS(0);
}
void WaterEffect::SetSpecularTexture(const Texture * specularTexture)
{
	specularTexture->BindPS(1);
}
void WaterEffect::SetDisplacementTexture(const Texture * displacementTexture)
{
	displacementTexture->BindVS(2);
	displacementTexture->BindPS(2);
}
void WaterEffect::SetNormalTexture(const Texture * normalTexture)
{
	normalTexture->BindPS(3);
}
