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
	mBoneTransformBuffer.Initialize();

	// Shaders
	mVertexShader.Initialize(fileName, BoneVertex::Format);
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
	mBoneTransformBuffer.Terminate();
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
	//mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);
	mShadowConstantBuffer.BindVS(4);
	mBoneTransformBuffer.BindVS(5);

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
	//mMaterialBuffer.UnbindVS(2);
	mMaterialBuffer.UnbindPS(2);
	mSettingsBuffer.UnbindVS(3);
	mSettingsBuffer.UnbindPS(3);
	mShadowConstantBuffer.UnbindVS(4);
	mBoneTransformBuffer.UnbindVS(5);

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

void StandardEffect::SetBoneTransforms(const std::vector<Math::Matrix4>& boneTransforms)
{
	for (size_t i = 0; i < boneTransforms.size(); i++)
	{
		mBoneTransform.boneTransforms[i] = boneTransforms[i];
	}
	mBoneTransformBuffer.Update(&mBoneTransform);
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
	mSettings.bumpMapWeight = 1.0f;
	mDisplacementMap.Initialize(fileName);
}
void StandardEffect::SetAOTexture(const std::filesystem::path & fileName)
{
	mSettings.aoMapWeight = 1.0f;
	mAmbientOcclusionMap.Initialize(fileName);
}
void StandardEffect::SetDepthTexture(const RenderTarget * target)
{
	target->BindPS(5);
}

void StandardEffect::SetDiffuseTexture(const Texture * diffuseTexture)
{
	diffuseTexture->BindPS(0);
}
void StandardEffect::SetNormalTexture(const Texture * normalTexture)
{
	normalTexture->BindPS(3);
}
void StandardEffect::SetSpecularTexture(const Texture * specularTexture)
{
	specularTexture->BindPS(1);
}
void StandardEffect::SetDisplacementTexture(const Texture * displacementTexture)
{
	displacementTexture->BindVS(2);
}
void StandardEffect::SetAOTexture(const Texture * aoTexture)
{
	aoTexture->BindPS(4);
}
void StandardEffect::SetDepthTexture(const Texture * depthTexture)
{
	depthTexture->BindPS(5);
}