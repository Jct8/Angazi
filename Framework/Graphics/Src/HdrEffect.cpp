#include "Precompiled.h"
#include "HdrEffect.h"

#include "MeshBuilder.h"

using namespace Angazi;
using namespace Angazi::Graphics;

META_DERIVED_BEGIN(HdrEffect, Effect)
	META_NO_FIELD
	META_NO_METHOD
META_CLASS_END

void HdrEffect::Initialize(const std::filesystem::path& fileName, const char* vshaderName, const char* pshaderName)
{
	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_F32);
	mScreenQuadBuffer.Initialize(MeshBuilder::CreateNDCQuad());
	mVertexShader.Initialize(fileName, VertexPX::Format, vshaderName);
	mPixelShader.Initialize(fileName, pshaderName);
	mSettingsBuffer.Initialize();
}

void HdrEffect::Terminate()
{
	mSettingsBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
}

void HdrEffect::BeginRender()
{
	mRenderTarget.BeginRender();
}

void HdrEffect::EndRender()
{
	mRenderTarget.EndRender();
}

void HdrEffect::RenderHdrQuad()
{
	auto graphicsSystem = GraphicsSystem::Get();
	mSettings.width = graphicsSystem->GetBackBufferWidth();
	mSettings.height = graphicsSystem->GetBackBufferHeight();
	mSettingsBuffer.Set(mSettings);
	mSettingsBuffer.BindPS(0);
	mRenderTarget.BindPS(0);
	mPixelShader.Bind();
	mVertexShader.Bind();
	mScreenQuadBuffer.Draw();
	mRenderTarget.UnbindPS(0);
}