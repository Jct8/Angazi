#pragma once

#include "Material.h"
#include "LightTypes.h"

namespace Angazi::Graphics
{
	class EnvironmentMap 
	{
	public:
		EnvironmentMap() {};
		~EnvironmentMap() = default;
		EnvironmentMap(const EnvironmentMap&) = delete;
		EnvironmentMap& operator=(const EnvironmentMap&) = delete;

		void Initialize(const std::filesystem::path& fileName = Angazi::Core::FilePath::GetAssetFilePath() + "Shaders/EnvironmentMap.fx");
		void Terminate();

		void Begin();
		void End();

	public:

		void SetWorldMatrix(const Math::Matrix4& world);
		void SetWVPMatrix(const Math::Matrix4& world, const Math::Matrix4& view, const Math::Matrix4& projection);
		void SetViewPosition(const Math::Vector3& viewPosition);
		void SetTransformData(const Math::Matrix4& world, const Math::Matrix4& view, const Math::Matrix4& projection, const Math::Vector3& viewPosition);

		void SetDirectionalLight(const DirectionalLight& light);
		void SetMaterial(const Material& material);

		void SetDiffuseTexture(const std::filesystem::path& fileName);
		void SetDiffuseTexture(const Texture* diffuseTexture);
		void SetCubeMapTexture(const std::filesystem::path& fileName);
		void SetCubeMapTexture(const Texture* cubeMapTexture);

		void SetBrightness(float brightness) { mSettings.brightness = brightness; };
		void SetEnvironmentRatio(float environmentRatio) { mSettings.environmentRatio = environmentRatio; };
		void SetReflectRefractRatio(float reflectRefractRatio) { mSettings.reflectRefractRatio = reflectRefractRatio; };
		void SetRefractionIndex(float refractionIndex) { mSettings.refractionIndex = refractionIndex; };

		void UpdateSettings() { mSettingsBuffer.Set(mSettings); };

	private:
		struct TransformData
		{
			Angazi::Math::Matrix4 world;
			Angazi::Math::Matrix4 wvp;
			Angazi::Math::Vector3 viewPosition;
			float padding =0.0f;
		};

		struct Settings
		{
			float brightness = 1.0f;
			float environmentRatio = 0.6f;
			float reflectRefractRatio = 0.5f;
			float refractionIndex = 1.0f/1.33f;
		};

		using TransformBuffer = Angazi::Graphics::TypedConstantBuffer<TransformData>;
		using LightBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Graphics::DirectionalLight>;
		using MaterialBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Graphics::Material>;
		using SettingsBuffer = Angazi::Graphics::TypedConstantBuffer<Settings>;

		TransformBuffer mTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;

		Settings mSettings;
		TransformData transformData;

		Angazi::Graphics::VertexShader mVertexShader;
		Angazi::Graphics::PixelShader  mPixelShader;

		Angazi::Graphics::Texture mDiffuseMap;
		Angazi::Graphics::Texture mCubeMap;
		Angazi::Graphics::Sampler mSampler;
	};
}