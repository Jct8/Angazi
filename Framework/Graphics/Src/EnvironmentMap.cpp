#include "Precompiled.h"
#include "EnvironmentMap.h"

#include "VertexTypes.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void EnvironmentMap::Initialize(const std::filesystem::path & fileName)
{
	// Constant Buffers
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	// Shaders
	mVertexShader.Initialize(fileName, Vertex::Format);
	mPixelShader.Initialize(fileName);

	// Sampler
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
}

void EnvironmentMap::Terminate()
{
	// Sampler
	mSampler.Terminate();

	// Shaders
	mPixelShader.Terminate();
	mVertexShader.Terminate();

	// Constant Buffers
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();

	// Textures
	mDiffuseMap.Terminate();
	mCubeMap.Terminate();
}

void EnvironmentMap::Begin()
{
	// Sampler
	mSampler.BindVS();
	mSampler.BindPS();

	// Constant Buffers
	mTransformBuffer.BindVS(0);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);
	mMaterialBuffer.BindPS(2);
	mSettingsBuffer.BindPS(3);

	// Maps
	mDiffuseMap.BindPS(0);
	mCubeMap.BindPS(1);

	// Shaders
	mVertexShader.Bind();
	mPixelShader.Bind();
}

void EnvironmentMap::End()
{
	// Constant Buffer
	mTransformBuffer.UnbindVS(0);
	mLightBuffer.UnbindVS(1);
	mLightBuffer.UnbindPS(1);
	mMaterialBuffer.UnbindPS(2);
	mSettingsBuffer.UnbindPS(3);

	// Textures
	mDiffuseMap.UnbindPS(0);
	mCubeMap.UnbindPS(1);
}

void EnvironmentMap::SetWorldMatrix(const Math::Matrix4 & world)
{
	transformData.world = Math::Transpose(world);
	mTransformBuffer.Update(&transformData);
}

void EnvironmentMap::SetWVPMatrix(const Math::Matrix4 & world, const Math::Matrix4 & view, const Math::Matrix4 & projection)
{
	transformData.wvp = Math::Transpose(world * view * projection);
	mTransformBuffer.Update(&transformData);
}

void EnvironmentMap::SetViewPosition(const Math::Vector3 & viewPosition)
{
	transformData.viewPosition = viewPosition;
	mTransformBuffer.Update(&transformData);
}

void EnvironmentMap::SetTransformData(const Math::Matrix4 & world, const Math::Matrix4 & view, const Math::Matrix4 & projection, const Math::Vector3 & viewPosition)
{
	transformData.world = Math::Transpose(world);
	transformData.wvp = Math::Transpose(world * view * projection);
	transformData.viewPosition = viewPosition;
	mTransformBuffer.Set(transformData);
}

void EnvironmentMap::SetDirectionalLight(const DirectionalLight & light)
{
	mLightBuffer.Set(light);
}

void EnvironmentMap::SetMaterial(const Material & material)
{
	mMaterialBuffer.Set(material);
}

void EnvironmentMap::SetDiffuseTexture(const std::filesystem::path & fileName)
{
	mDiffuseMap.Initialize(fileName);
}

void EnvironmentMap::SetDiffuseTexture(const Texture * diffuseTexture)
{
	diffuseTexture->BindPS(0);
}

void EnvironmentMap::SetCubeMapTexture(const std::filesystem::path & fileName)
{
	mCubeMap.Initialize(fileName);
}

void EnvironmentMap::SetCubeMapTexture(const Texture * cubeMapTexture)
{
	cubeMapTexture->BindPS(1);
}
