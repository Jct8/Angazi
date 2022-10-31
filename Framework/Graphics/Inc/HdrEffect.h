#pragma once

#include "Mesh.h"
#include "Effect.h"

namespace Angazi::Graphics
{
	class HdrEffect : public Effect
	{
	public:
		META_CLASS_DECLARE;

		HdrEffect() : Effect(EffectType::PbrType) {}
		~HdrEffect() = default;
		HdrEffect(const HdrEffect&) = delete;
		HdrEffect& operator=(const HdrEffect&) = delete;
	public:
		void Initialize(const std::filesystem::path& fileName, const char* vshaderName, const char* pshaderName );
		void Initialize(const std::filesystem::path& fileName = Angazi::Core::FilePath::GetAssetFilePath() + "Shaders/HDR.fx") override { Initialize(fileName,"VS","PS"); }

		void Terminate() override;

		void Begin() override { BeginRender(); }
		void End() override { EndRender(); }

		void BeginRender();
		void EndRender();

		void EnableHDR(bool enable) { mSettings.useHDR = enable; }
		void EnableGammaCorrection(bool enable) { mSettings.useGammaCorrection = enable; }
		void EnableEdgeDetection(bool enable) { mSettings.useEdgeDetection = enable; }
		void SetExposure(float exposure) { mSettings.exposure = exposure; }

		void RenderHdrQuad();

		void* GetRenderTargetData() { return mRenderTarget.GetShaderResourceView(); }
	private:
		struct Settings
		{
			int useHDR = true;
			int useGammaCorrection = true;
			int useEdgeDetection = false;
			float exposure = 2.0f;
			float width = 1280.0f;
			float height = 720.0f;
			float padding[2];
		};

		Angazi::Graphics::TypedConstantBuffer<Settings> mSettingsBuffer;
		Settings mSettings;

		Angazi::Graphics::RenderTarget mRenderTarget;
		Angazi::Graphics::MeshBuffer mScreenQuadBuffer;
		Angazi::Graphics::VertexShader mVertexShader;
		Angazi::Graphics::PixelShader mPixelShader;
	};
}