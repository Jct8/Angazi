#include "Precompiled.h"
#include "ShadowEffect.h"

#include "SimpleDraw.h"
#include "VertexTypes.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	void SimpleDrawCamera(const Camera& camera)
	{
		auto defaultMatView = camera.GetViewMatrix();
		Math::Vector3 cameraPosition = camera.GetPosition();
		Math::Vector3 cameraRight = { defaultMatView._11, defaultMatView._21, defaultMatView._31 };
		Math::Vector3 cameraUp = { defaultMatView._12, defaultMatView._22, defaultMatView._32 };
		Math::Vector3 cameraLook = { defaultMatView._13, defaultMatView._23, defaultMatView._33 };
		SimpleDraw::AddSphere(cameraPosition, 0.1f, Colors::White, false, 6, 8);
		SimpleDraw::AddLine(cameraPosition, cameraPosition + cameraRight, Colors::Red);
		SimpleDraw::AddLine(cameraPosition, cameraPosition + cameraUp, Colors::Green);
		SimpleDraw::AddLine(cameraPosition, cameraPosition + cameraLook, Colors::Blue);
	}
}

void ShadowEffect::Initialize(const std::filesystem::path & fileName)
{
	mLightCamera.SetDirection(Math::Normalize({ 1.0f, -1.0f, 1.0f }));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(200.0f);
	mLightCamera.SetFov(1.0f);
	mLightCamera.SetAspectRatio(1.0f);

	constexpr uint32_t depthMapSize = 8000;//4096;
	auto graphicsSystem = GraphicsSystem::Get();
	//mDepthMapRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	mDepthMapRenderTarget.Initialize(depthMapSize, depthMapSize, RenderTarget::Format::RGBA_U32);
	mDepthMapVertexShader.Initialize(fileName, BoneVertex::Format);
	mDepthMapPixelShader.Initialize(fileName);
	mDepthMapConstantBuffer.Initialize();

	mBoneTransformBuffer.Initialize();
	mSettingsBuffer.Initialize();
}

void ShadowEffect::Terminate()
{
	mSettingsBuffer.Terminate();
	mBoneTransformBuffer.Terminate();

	mDepthMapConstantBuffer.Terminate();
	mDepthMapPixelShader.Terminate();
	mDepthMapVertexShader.Terminate();
	mDepthMapRenderTarget.Terminate();
	mDepthTexture.Terminate();
}

void ShadowEffect::Begin()
{
	mDepthMapRenderTarget.BeginRender();
	mDepthMapPixelShader.Bind();
	mDepthMapVertexShader.Bind();
	mDepthMapConstantBuffer.BindVS(0);

	mBoneTransformBuffer.BindVS(1);
	mSettingsBuffer.BindVS(2);
}

void ShadowEffect::End()
{
	mDepthMapConstantBuffer.UnbindVS(0);
	mBoneTransformBuffer.UnbindVS(1);
	mSettingsBuffer.UnbindVS(2);

	mDepthMapRenderTarget.EndRender();
}

void ShadowEffect::SetLightDirection(const Math::Vector3 & direction, const Camera& mCurrentCamera)
{
	mLightCamera.SetDirection(direction);
	mViewFrustumVertices =
	{
		// Near plane
		{ -1.0f, -1.0f, 0.0f },
		{ -1.0f,  1.0f, 0.0f },
		{  1.0f,  1.0f, 0.0f },
		{  1.0f, -1.0f, 0.0f },

		// Far plane
		{ -1.0f, -1.0f, 1.0f },
		{ -1.0f,  1.0f, 1.0f },
		{  1.0f,  1.0f, 1.0f },
		{  1.0f, -1.0f, 1.0f },
	};
	auto defaultMatView = mCurrentCamera.GetViewMatrix();
	auto defaultMatProj = mCurrentCamera.GetPerspectiveMatrix();
	auto invViewProj = Math::Inverse(defaultMatView * defaultMatProj);
	for (auto& vertex : mViewFrustumVertices)
	{
		vertex = TransformCoord(vertex, invViewProj);
	}

	auto lightLook = mLightCamera.GetDirection();
	auto lightSide = Math::Normalize(Cross(Math::Vector3::YAxis, lightLook));
	auto lightUp = Math::Normalize(Cross(lightLook, lightSide));
	float minX = FLT_MAX, maxX = -FLT_MAX;
	float minY = FLT_MAX, maxY = -FLT_MAX;
	float minZ = FLT_MAX, maxZ = -FLT_MAX;
	for (auto& vertex : mViewFrustumVertices)
	{
		float projectX = Dot(lightSide, vertex);
		minX = Math::Min(minX, projectX);
		maxX = Math::Max(maxX, projectX);
		float projectY = Dot(lightUp, vertex);
		minY = Math::Min(minY, projectY);
		maxY = Math::Max(maxY, projectY);
		float projectZ = Dot(lightLook, vertex);
		minZ = Math::Min(minZ, projectZ);
		maxZ = Math::Max(maxZ, projectZ);
	}
	mLightCamera.SetPosition(
		lightSide * (minX + maxX) * 0.5f +
		lightUp * (minY + maxY) * 0.5f +
		lightLook * (minZ + maxZ) * 0.5f
	);
	mLightCamera.SetNearPlane(minZ - mCurrentCamera.GetFarPlane());
	mLightCamera.SetFarPlane(maxZ + mCurrentCamera.GetFarPlane());
	mLightProjectionMatrix = mLightCamera.GetOrthographicMatrix(maxX - minX, maxY - minY);
}

void ShadowEffect::SetWorldMatrix(const Math::Matrix4 & world)
{
	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightProjectionMatrix;// mLightCamera.GetPerspectiveMatrix();

	auto wvp = Math::Transpose(world * matViewLight * matProjLight);
	mDepthMapConstantBuffer.Update(&wvp);
}

void ShadowEffect::SetBoneTransforms(const std::vector<Math::Matrix4>& boneTransforms)
{
	for (size_t i = 0; i < boneTransforms.size(); i++)
	{
		mBoneTransform.boneTransforms[i] = boneTransforms[i];
	}
	mBoneTransformBuffer.Update(&mBoneTransform);
}