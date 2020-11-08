#include "Precompiled.h"
#include "PbrEffect.h"

#include "VertexTypes.h"
#include "MeshBuilder.h"

using namespace Angazi;
using namespace Angazi::Graphics;

META_DERIVED_BEGIN(PbrEffect, Effect)
	META_NO_FIELD
META_CLASS_END

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
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
	mBlendState.Initialize(BlendState::Mode::Opaque);

	// Create BRDF Texture
	VertexShader vertexShader;
	PixelShader pixelShader;
	MeshBuffer meshBuffer;

	mBRDFlutTexture.Initialize(512, 512, RenderTarget::Format::RGBA_F16);
	meshBuffer.Initialize(MeshBuilder::CreateNDCQuad());
	vertexShader.Initialize("../../Assets/Shaders/BRDF.fx", VertexPX::Format);
	pixelShader.Initialize("../../Assets/Shaders/BRDF.fx");
	mBRDFlutTexture.BeginRender();
	pixelShader.Bind();
	vertexShader.Bind();
	meshBuffer.Draw();
	mBRDFlutTexture.EndRender();

	meshBuffer.Terminate();
	pixelShader.Terminate();
	vertexShader.Terminate();
}
void PbrEffect::Terminate()
{
	mBRDFlutTexture.Terminate();

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
	mIrradienceMap.Terminate();
	mPreFilterMap.Terminate();
	mBRDFlutTexture.Terminate();
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
	mIrradienceMap.BindPS(7);
	mPreFilterMap.BindPS(8);
	mBRDFlutTexture.BindPS(9);

	if (!mAmbientOcclusionMap.GetShaderResourceView())
		mSettings.aoMapWeight = 0.0f;
	if (!mDisplacementMap.GetShaderResourceView())
		mSettings.bumpMapWeight = 0.0f;
	if (!mNormalMap.GetShaderResourceView())
		mSettings.normalMapWeight = 0.0f;

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
	mIrradienceMap.UnbindVS(7);
	mPreFilterMap.UnbindPS();

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
	mSettings.bumpMapWeight = 0.1f;
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
void PbrEffect::SetIrradianceMap(const std::filesystem::path& fileName)
{
	mIrradienceMap.Initialize(fileName);
}
void PbrEffect::SetPreFilterMap(const std::filesystem::path& fileName)
{
	mPreFilterMap.Initialize(fileName);
}

void PbrEffect::SetDiffuseTexture(const Texture * diffuseTexture)
{
	diffuseTexture->BindPS(0);
}
void PbrEffect::SetNormalTexture(const Texture * normalTexture)
{
	mSettings.normalMapWeight = 1.0f;
	normalTexture->BindPS(2);
}
void PbrEffect::SetDisplacementTexture(const Texture * displacementTexture)
{
	displacementTexture->BindVS(1);
}
void PbrEffect::SetAOTexture(const Texture * aoTexture)
{
	mSettings.aoMapWeight = 1.0f;
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
void PbrEffect::SetIrradianceMap(const Texture* irradianceMap)
{
	irradianceMap->BindPS(7);
}
void PbrEffect::SetPreFilterMap(const Texture* preFilterMap)
{
	preFilterMap->BindPS(8);
}
void PbrEffect::SetBRDFlutTexture(const Texture* brdfLutTexture)
{
	brdfLutTexture->BindPS(9);
}