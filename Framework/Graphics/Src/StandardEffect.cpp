#include "Precompiled.h"

#include "VertexTypes.h"
#include "Sampler.h"
#include "BlendState.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "StandardEffect.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void StandardEffect::Initialize(const std::filesystem::path & fileName)
{
	// Constant Buffers
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mShadowConstantBuffer.Initialize();

	// Shaders
	mVertexShader.Initialize(fileName, Vertex::Format);
	mPixelShader.Initialize(fileName);

	// 
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mBlendState.Initialize(BlendState::Mode::Additive);

}
void StandardEffect::Terminate()
{
	mBlendState.Terminate();
	mSampler.Terminate();

	// Shaders
	mPixelShader.Terminate();
	mVertexShader.Terminate();

	// Constant Buffers
	mShadowConstantBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();

	// Textures
	mDiffuseMap.Terminate();
	mSpecularMap.Terminate();
	mDisplacementMap.Terminate();
	mNormalMap.Terminate();
	mAmbientOcclusionMap.Terminate();
}
void StandardEffect::Begin()
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
	mNormalMap.BindPS(3);
	mAmbientOcclusionMap.BindPS(4);

	mVertexShader.Bind();
	mPixelShader.Bind();
}
void StandardEffect::End()
{
	// Constant Buffer
	mTransformBuffer.UnbindVS(0);
	mLightBuffer.UnbindVS(1);
	mLightBuffer.UnbindPS(1);
	mMaterialBuffer.UnbindVS(2);
	mMaterialBuffer.UnbindPS(2);
	mSettingsBuffer.UnbindVS(3);
	mSettingsBuffer.UnbindPS(3);

	// Textures
	mDiffuseMap.UnbindPS(0);
	mSpecularMap.UnbindPS(1);
	mDisplacementMap.UnbindVS(2);
	mNormalMap.UnbindPS(3);
	mAmbientOcclusionMap.UnbindPS(4);
}

void StandardEffect::SetWorldMatrix(const Math::Matrix4 & world)
{
	transformData.world = Math::Transpose(world);
	mTransformBuffer.Update(&transformData);
}
void StandardEffect::SetWVPMatrix(const Math::Matrix4& world, const Math::Matrix4& view, const Math::Matrix4& projection)
{
	transformData.wvp = Math::Transpose(world * view * projection);
	mTransformBuffer.Update(&transformData);
}
void StandardEffect::SetViewProjection(const Math::Vector3 & viewProjection)
{
	transformData.viewPosition = viewProjection;
	mTransformBuffer.Update(&transformData);
}

void StandardEffect::SetDirectionalLight(const DirectionalLight & light)
{
	mLightBuffer.Update(&light);
}
void StandardEffect::SetMaterial(const Material & material)
{
	mMaterialBuffer.Update(&material);
}

void StandardEffect::SetDiffuseTexture(const std::filesystem::path & fileName)
{
	mDiffuseMap.Initialize(fileName);
}
void StandardEffect::SetNormalTexture(const std::filesystem::path & fileName)
{
	mSettings.normalMapWeight = 1.0f;
	mNormalMap.Initialize(fileName);
}
void StandardEffect::SetSpecularTexture(const std::filesystem::path & fileName)
{
	mSettings.specularMapWeight = 1.0f;
	mSpecularMap.Initialize(fileName);
}
void StandardEffect::SetDisplacementTexture(const std::filesystem::path & fileName)
{
	mDisplacementMap.Initialize(fileName);
}
void StandardEffect::SetAOTexture(const std::filesystem::path & fileName)
{
	mSettings.aoMapWeight = 1.0f;
	mAmbientOcclusionMap.Initialize(fileName);
}

void StandardEffect::SetDepthTexture(const Texture * depthTexture)
{
	depthTexture->BindPS(5);
}

void StandardEffect::SetDepthTexture(const RenderTarget * target)
{
	target->BindPS(5);
}
