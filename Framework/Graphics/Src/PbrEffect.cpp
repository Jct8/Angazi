#include "Precompiled.h"
#include "PbrEffect.h"

#include "VertexTypes.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void PbrEffect::Initialize(const std::filesystem::path & fileName)
{
	// Constant Buffers
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mShadowConstantBuffer.Initialize();
	mBoneTransformBuffer.Initialize();
	mClippingConstantBuffer.Initialize();

	// Shaders
	mVertexShader.Initialize(fileName, BoneVertex::Format);
	mPixelShader.Initialize(fileName);

	// 
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mBlendState.Initialize(BlendState::Mode::Additive);

}
void PbrEffect::Terminate()
{
	mBlendState.Terminate();
	mSampler.Terminate();

	// Shaders
	mPixelShader.Terminate();
	mVertexShader.Terminate();

	// Constant Buffers
	mClippingConstantBuffer.Terminate();
	mBoneTransformBuffer.Terminate();
	mShadowConstantBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();

	// Textures
	mDiffuseMap.Terminate();
	mDisplacementMap.Terminate();
	mNormalMap.Terminate();
	mAmbientOcclusionMap.Terminate();
	mMetallicMap.Terminate();
	mRoughnessMap.Terminate();
}
void PbrEffect::Begin()
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
	mClippingConstantBuffer.BindVS(6);

	// Maps
	mDiffuseMap.BindPS(0);
	mDisplacementMap.BindVS(1);
	mNormalMap.BindPS(2);
	mAmbientOcclusionMap.BindPS(3);
	mMetallicMap.BindPS(5);
	mRoughnessMap.BindPS(6);

	mVertexShader.Bind();
	mPixelShader.Bind();
}
void PbrEffect::End()
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
	mClippingConstantBuffer.UnbindVS(6);

	// Textures
	mDiffuseMap.UnbindPS(0);
	mDisplacementMap.UnbindVS(1);
	mNormalMap.UnbindPS(2);
	mAmbientOcclusionMap.UnbindPS(3);
	mMetallicMap.UnbindPS(5);
	mRoughnessMap.UnbindPS(6);
}

void PbrEffect::SetWorldMatrix(const Math::Matrix4 & world)
{
	transformData.world = Math::Transpose(world);
	mTransformBuffer.Update(&transformData);
}
void PbrEffect::SetWVPMatrix(const Math::Matrix4& world, const Math::Matrix4& view, const Math::Matrix4& projection)
{
	transformData.wvp = Math::Transpose(world * view * projection);
	mTransformBuffer.Update(&transformData);
}
void PbrEffect::SetViewPosition(const Math::Vector3 & viewPosition)
{
	transformData.viewPosition = viewPosition;
	mTransformBuffer.Update(&transformData);
}
void PbrEffect::SetTransformData(const Math::Matrix4 & world, const Math::Matrix4 & view, const Math::Matrix4 & projection, const Math::Vector3 & viewPosition)
{
	transformData.world = Math::Transpose(world);
	transformData.wvp = Math::Transpose(world * view * projection);
	transformData.viewPosition = viewPosition;
	mTransformBuffer.Update(&transformData);
}

void PbrEffect::SetDirectionalLight(const DirectionalLight & light)
{
	mLightBuffer.Update(&light);
}
void PbrEffect::SetMaterial(const Material & material)
{
	mMaterialBuffer.Update(&material);
}
void PbrEffect::SetBoneTransforms(const std::vector<Math::Matrix4>& boneTransforms)
{
	for (size_t i = 0; i < boneTransforms.size(); i++)
		mBoneTransform.boneTransforms[i] = boneTransforms[i];
	mBoneTransformBuffer.Set(mBoneTransform);
}
void PbrEffect::SetClippingPlane(const Math::Vector4& plane)
{
	mClipping.plane = plane;
	mClippingConstantBuffer.Set(mClipping);
}

void PbrEffect::SetDiffuseTexture(const std::filesystem::path & fileName)
{
	mDiffuseMap.Initialize(fileName);
}
void PbrEffect::SetNormalTexture(const std::filesystem::path & fileName)
{
	mSettings.normalMapWeight = 1.0f;
	mNormalMap.Initialize(fileName);
}
void PbrEffect::SetDisplacementTexture(const std::filesystem::path & fileName)
{
	mSettings.bumpMapWeight = 1.0f;
	mDisplacementMap.Initialize(fileName);
}
void PbrEffect::SetAOTexture(const std::filesystem::path & fileName)
{
	mSettings.aoMapWeight = 1.0f;
	mAmbientOcclusionMap.Initialize(fileName);
}
void PbrEffect::SetDepthTexture(const RenderTarget * target)
{
	target->BindPS(4);
}
void PbrEffect::SetMetallicTexture(const std::filesystem::path& fileName)
{
	mMetallicMap.Initialize(fileName);
}
void PbrEffect::SetRoughnessTexture(const std::filesystem::path& fileName)
{
	mRoughnessMap.Initialize(fileName);
}

void PbrEffect::SetDiffuseTexture(const Texture * diffuseTexture)
{
	diffuseTexture->BindPS(0);
}
void PbrEffect::SetNormalTexture(const Texture * normalTexture)
{
	normalTexture->BindPS(2);
}
void PbrEffect::SetDisplacementTexture(const Texture * displacementTexture)
{
	displacementTexture->BindVS(1);
}
void PbrEffect::SetAOTexture(const Texture * aoTexture)
{
	aoTexture->BindPS(3);
}
void PbrEffect::SetDepthTexture(const Texture * depthTexture)
{
	depthTexture->BindPS(4);
}
void PbrEffect::SetMetallicTexture(const Texture* metallicMap)
{
	metallicMap->BindPS(5);
}
void PbrEffect::SetRoughnessTexture(const Texture* roughnessMap)
{
	roughnessMap->BindPS(6);
}