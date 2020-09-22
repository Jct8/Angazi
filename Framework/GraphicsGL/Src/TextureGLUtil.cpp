#include "Precompiled.h"
#include "TextureGLUtil.h"

#ifdef ENABLE_OPENGL

#include "RenderTargetGL.h"
#include "PixelShaderGL.h"
#include "VertexShaderGL.h"
#include "MeshBufferGL.h"
#include "Graphics/Inc/MeshBuilder.h"
#include "Graphics/Inc/Camera.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Math;

namespace
{
	const Math::Matrix4 cubeLookDir[] =
	{
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 1.0f,  0.0f,  0.0f }, { 0.0f, -1.0f,  0.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({-1.0f,  0.0f,  0.0f }, { 0.0f, -1.0f,  0.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 0.0f, -1.0f,  0.0f }, { 0.0f,  0.0f,  1.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 0.0f,  1.0f,  0.0f }, { 0.0f,  0.0f, -1.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 0.0f,  0.0f, -1.0f }, { 0.0f, -1.0f,  0.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 0.0f,  0.0f,  1.0f }, { 0.0f, -1.0f,  0.0f }))
	};
}

uint32_t Angazi::Graphics::TextureUtil::CreateCubeMapFromTexture(uint32_t texture, const std::filesystem::path& shaderFilePath, uint32_t cubeLength)
{
	RenderTarget renderTarget;
	VertexShader vertexShader;
	PixelShader pixelShader;
	MeshBuffer meshBuffer;
	TypedConstantBuffer<Math::Matrix4> tranformBuffer;

	Camera camera;

	renderTarget.Initialize(cubeLength, cubeLength, RenderTarget::Format::RGBA_F16);
	meshBuffer.Initialize(MeshBuilder::CreateInnerCubeP());
	vertexShader.Initialize(shaderFilePath, VertexP::Format);
	pixelShader.Initialize(shaderFilePath);
	tranformBuffer.Initialize();

	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(10.0f);
	camera.SetFov(90.0f * Math::Constants::DegToRad);
	camera.SetAspectRatio(1.0f);

	uint32_t cubemap;
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	for (uint32_t i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, cubeLength, cubeLength, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	pixelShader.Bind();
	vertexShader.Bind();
	auto matProj = camera.GetPerspectiveMatrix();

	glBindTextureUnit(0, texture);
	renderTarget.BeginRender();
	for (uint32_t i = 0; i < 6; ++i)
	{
		auto matView = cubeLookDir[i];

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		tranformBuffer.Set(Math::Transpose(matView * matProj));
		tranformBuffer.BindVS(0);
		meshBuffer.Draw();
	}
	renderTarget.EndRender();

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	tranformBuffer.Terminate();
	meshBuffer.Terminate();
	pixelShader.Terminate();
	vertexShader.Terminate();
	renderTarget.Terminate();

	return cubemap;
}

uint32_t Angazi::Graphics::TextureUtil::CreatePreFilteredCubeMap(uint32_t texture, const std::filesystem::path& shaderFilePath, uint32_t cubeLength)
{
	RenderTarget renderTarget;
	VertexShader vertexShader;
	PixelShader pixelShader;
	MeshBuffer meshBuffer;
	TypedConstantBuffer<Math::Matrix4> tranformBuffer;
	struct Settings
	{
		float roughness;
		float padding[3];
	};
	TypedConstantBuffer<Settings> roughnessBuffer;
	Camera camera;

	renderTarget.Initialize(cubeLength, cubeLength, RenderTarget::Format::RGBA_F16);
	meshBuffer.Initialize(MeshBuilder::CreateInnerCubeP());
	vertexShader.Initialize(shaderFilePath, VertexP::Format);
	pixelShader.Initialize(shaderFilePath);
	tranformBuffer.Initialize();
	roughnessBuffer.Initialize();

	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(10.0f);
	camera.SetFov(90.0f * Math::Constants::DegToRad);
	camera.SetAspectRatio(1.0f);

	uint32_t prefilterMap;
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (uint32_t i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, cubeLength, cubeLength, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	pixelShader.Bind();
	vertexShader.Bind();
	auto matProj = camera.GetPerspectiveMatrix();

	uint32_t maxMipLevels = 5;
	renderTarget.BeginRender();
	glBindTextureUnit(0, texture);
	for (uint32_t mipLevel = 0; mipLevel < maxMipLevels; ++mipLevel)
	{
		uint32_t mipWidth = static_cast<uint32_t>(cubeLength * std::pow(0.5f, mipLevel));
		uint32_t mipHeight = static_cast<uint32_t>(cubeLength * std::pow(0.5f, mipLevel));
		renderTarget.Initialize(mipWidth, mipHeight, RenderTarget::Format::RGBA_F16);
		float roughness = static_cast<float>(mipLevel) / static_cast<float>(maxMipLevels - 1);

		Settings roughnessSettings;
		roughnessSettings.roughness = roughness;
		roughnessBuffer.Set(roughnessSettings);
		roughnessBuffer.BindPS(1);

		renderTarget.BeginRender();
		for (uint32_t i = 0; i < 6; ++i)
		{
			auto matView = cubeLookDir[i];
			tranformBuffer.Set(Math::Transpose(matView * matProj));
			tranformBuffer.BindVS(0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mipLevel);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			meshBuffer.Draw();
		}
		renderTarget.EndRender();
		renderTarget.Terminate();
	}

	roughnessBuffer.Terminate();
	tranformBuffer.Terminate();
	meshBuffer.Terminate();
	pixelShader.Terminate();
	vertexShader.Terminate();
	renderTarget.Terminate();

	return prefilterMap;
}

#endif