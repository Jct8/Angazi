#pragma once

#include "Camera.h"
#include "Effect.h"

namespace Angazi::Graphics
{
	class ShadowEffect : public Effect
	{
	public:
		META_CLASS_DECLARE;

		ShadowEffect():Effect(EffectType::ShadowType) {};
		~ShadowEffect() = default;
		ShadowEffect(const ShadowEffect&) = delete;
		ShadowEffect& operator=(const ShadowEffect&) = delete;

		void Initialize(const std::filesystem::path& fileName = Angazi::Core::FilePath::GetAssetFilePath() + "Shaders/DepthMap.fx") override;
		void Terminate() override;

		void Begin() override;
		void End() override;

		void BeginWithoutTarget();
		void EndWithoutTarget();
	public:
		void SetLightDirection(const Math::Vector3& direction, const Angazi::Graphics::Camera& mCurrentCamera);
		void SetWorldMatrix(const Math::Matrix4& world);
		void SetTransformData(const Math::Matrix4& world, const Math::Matrix4& view, const Math::Matrix4& projection, const Math::Vector3& viewPosition);
		void SetBoneTransforms(const std::vector<Math::Matrix4>& boneTransforms);

		void SetSkinnedMesh(bool isSkinnedMesh) { mSettings.isSkinnedMesh = isSkinnedMesh == true ? 1.0f : 0.0f; };
		void UpdateSettings() { mSettingsBuffer.Update(&mSettings); };

		Angazi::Graphics::Texture* GetDepthTexture() { return &mDepthTexture; };
		Angazi::Graphics::RenderTarget* GetRenderTarget() { return &mDepthMapRenderTarget; };
		Angazi::Math::Matrix4 GetVPMatrix() { return  mLightCamera.GetViewMatrix() *mLightProjectionMatrix; };

	private:
		struct BoneTransform
		{
			Math::Matrix4 boneTransforms[256];
		};

		struct Settings
		{
			float isSkinnedMesh = 0.0f;
			float padding[3]{};
		};

		using DepthMapConstantBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Math::Matrix4>;
		using BoneTransformBuffer = Angazi::Graphics::TypedConstantBuffer<BoneTransform>;
		using SettingsBuffer = Angazi::Graphics::TypedConstantBuffer<Settings>;

		//Shadow
		Angazi::Graphics::RenderTarget mDepthMapRenderTarget;
		Angazi::Graphics::VertexShader mDepthMapVertexShader;
		Angazi::Graphics::PixelShader mDepthMapPixelShader;
		DepthMapConstantBuffer mDepthMapConstantBuffer;

		//Settings
		SettingsBuffer mSettingsBuffer;
		Settings mSettings;

		//Animation
		BoneTransform mBoneTransform;
		BoneTransformBuffer mBoneTransformBuffer;

		std::vector<Angazi::Math::Vector3> mViewFrustumVertices;
		Angazi::Graphics::Camera mLightCamera;
		Angazi::Math::Matrix4 mLightProjectionMatrix;
		Angazi::Graphics::Texture mDepthTexture;

		bool showDebugLines = false;
	};
}