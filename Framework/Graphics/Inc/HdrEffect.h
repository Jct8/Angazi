#pragma once

#include "Mesh.h"

namespace Angazi::Graphics
{
	class HdrEffect
	{
	public:
		HdrEffect() = default;
		~HdrEffect() = default;
		HdrEffect(const HdrEffect&) = delete;
		HdrEffect& operator=(const HdrEffect&) = delete;
	public:
		void Initialize(const std::filesystem::path& fileName = "../../Assets/Shaders/HDR.fx", const char* vshaderName = "VS", const char* pshaderName = "PS");
		void Terminate();

		void BeginRender();
		void EndRender();

		void EnableHDR(bool enable) { mSettings.useHDR = enable; }
		void EnableGammaCorrection(bool enable) { mSettings.useGammaCorrection = enable; }
		void SetExposure(float exposure) { mSettings.exposure = exposure; }

		void RenderHdrQuad();
	private:
		struct Settings
		{
			int useHDR = true;
			int useGammaCorrection = true;
			float exposure = 1.0f;
			float padding;
		};

		Angazi::Graphics::TypedConstantBuffer<Settings> mSettingsBuffer;
		Settings mSettings;

		Angazi::Graphics::RenderTarget mRenderTarget;
		Angazi::Graphics::MeshBuffer mScreenQuadBuffer;
		Angazi::Graphics::VertexShader mVertexShader;
		Angazi::Graphics::PixelShader mPixelShader;
	};
}