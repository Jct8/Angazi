#pragma once

#include "Mesh.h"
#include "Effect.h"

namespace Angazi::Graphics
{
	class PostProcessingEffect
	{
	public:
		PostProcessingEffect() = default;
		~PostProcessingEffect() = default;
		PostProcessingEffect(const PostProcessingEffect&) = delete;
		PostProcessingEffect& operator=(const PostProcessingEffect&) = delete;
	public:
		void Initialize(const std::filesystem::path& fileName, const char* vshaderName, const char* pshaderName);
		void Terminate();

		void BeginRender();
		void EndRender();

		void PostProcess();

	private:
		Angazi::Graphics::Sampler mSampler;
		Angazi::Graphics::RenderTarget mRenderTarget;
		Angazi::Graphics::MeshPX mScreenQuad;
		Angazi::Graphics::MeshBuffer mScreenQuadBuffer;
		Angazi::Graphics::VertexShader mPostProcessingVertexShader;
		Angazi::Graphics::PixelShader mPostProcessingPixelShader;
	};
}