#include "Precompiled.h"
#include "ShadowEffect.h"
#include "VertexTypes.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void ShadowEffect::Initialize(const std::filesystem::path & fileName)
{
	mLightCamera.SetDirection(Math::Normalize({ 1.0f, -1.0f, 1.0f }));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(200.0f);
	mLightCamera.SetFov(1.0f);
	mLightCamera.SetAspectRatio(1.0f);

	constexpr uint32_t depthMapSize = 4096;
	mDepthMapRenderTarget.Initialize(depthMapSize, depthMapSize, RenderTarget::Format::RGBA_U32);
	mDepthMapVertexShader.Initialize(fileName, Vertex::Format);
	mDepthMapPixelShader.Initialize(fileName);
	mDepthMapConstantBuffer.Initialize();
}

void ShadowEffect::Terminate()
{
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
}

void ShadowEffect::End()
{
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
	mLightCamera.SetNearPlane(minZ - 300.0f);
	mLightCamera.SetFarPlane(maxZ);
	mLightProjectionMatrix = mLightCamera.GetOrthographicMatrix(maxX - minX, maxY - minY);
}

void ShadowEffect::SetWorldMatrix(const Math::Matrix4 & world)
{
	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightProjectionMatrix;// mLightCamera.GetPerspectiveMatrix();

	auto wvp = Math::Transpose(world * matViewLight * matProjLight);
	mDepthMapConstantBuffer.Update(&wvp);
}
