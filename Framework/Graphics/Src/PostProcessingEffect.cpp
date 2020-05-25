#include "Precompiled.h"
#include "PostProcessingEffect.h"
#include "GraphicsSystem.h"
#include "MeshBuilder.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void PostProcessingEffect::Initialize(const std::filesystem::path & fileName, const char * vshaderName, const char * pshaderName)
{
	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	mScreenQuad = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mScreenQuad);
	mPostProcessingVertexShader.Initialize(fileName, VertexPX::Format, vshaderName);
	mPostProcessingPixelShader.Initialize(fileName, pshaderName);
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
}

void PostProcessingEffect::Terminate()
{
	mSampler.Terminate();
	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
}

void PostProcessingEffect::BeginRender()
{
	mRenderTarget.BeginRender();
}

void PostProcessingEffect::EndRender()
{
	mRenderTarget.EndRender();
}

void PostProcessingEffect::PostProcess()
{
	mRenderTarget.BindPS(0);
	mPostProcessingPixelShader.Bind();
	mPostProcessingVertexShader.Bind();
	mSampler.BindPS();
	mScreenQuadBuffer.Draw();
	mRenderTarget.UnbindPS(0);
}
