#pragma once

namespace Angazi::Graphics
{
	struct DirectionalLight;
	struct Material;
	class Camera;

	class WaterEffect
	{
	public:
		WaterEffect() = default;
		~WaterEffect() = default;
		WaterEffect(const WaterEffect&) = delete;
		WaterEffect& operator=(const WaterEffect&) = delete;

		void Initialize(const std::filesystem::path& fileName = Angazi::Core::FilePath::GetAssetFilePath() + "Shaders/Water.fx");
		void Terminate();

		void Update(float deltaTime);

	public:
		void BeginRefraction();
		void EndRefraction();

		void BeginReflection(Camera &camera, float waterHeight);
		void EndReflection(Camera &camera);

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
		void SetSpecularTexture(const std::filesystem::path& fileName);
		void SetDisplacementTexture(const std::filesystem::path& fileName);
		void SetNormalTexture(const std::filesystem::path& fileName);
		//void SetDepthTexture(const RenderTarget* target);

		void SetDiffuseTexture(const Texture* diffuseTexture);
		void SetSpecularTexture(const Texture* specularTexture);
		void SetDisplacementTexture(const Texture* displacementTexture);
		void SetNormalTexture(const Texture* normalTexture);
		//void SetDepthTexture(const Texture* depthTexture);

		void SetSpecularMapWeight(float weight) { mSettings.specularMapWeight = weight; }
		void SetWaterDisplacement(float weight) { mSettings.bumpMapWeight = weight; }
		void SetNormalMapWeight(float weight) { mSettings.normalMapWeight = weight; }
		void SetBrightness(float brightness) { mSettings.brightness = brightness; };
		void SetMovement(float movement) { mSettings.movement = movement; };
		void SetMovementSpeed(float movementSpeed) { mSettings.movementSpeed = movementSpeed; };
		void SetReflectivePower(float reflectivePower) { mSettings.reflectivePower = reflectivePower; };

		void UpdateSettings() { mSettingsBuffer.Update(&mSettings); };

		void * GetRefractionTexture() const { return mRefractionRenderTarget.GetShaderResourceView(); }
		void * GetReflectionTexture() const { return mReflectionRenderTarget.GetShaderResourceView(); }
	private:
		struct TransformData
		{
			Angazi::Math::Matrix4 world;
			Angazi::Math::Matrix4 wvp;
			Angazi::Math::Vector3 viewPosition;
			float padding;
		};

		struct Settings
		{
			float specularMapWeight = 1.0f;
			float bumpMapWeight = 0.4f;
			float normalMapWeight =1.0f;
			float brightness = 1.0f;
			float movement = 0.0f;
			float movementSpeed = 0.005f;
			float reflectivePower = 0.5f;
			float padding[1];
		};

		using TransformBuffer = Angazi::Graphics::TypedConstantBuffer<TransformData>;
		using LightBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Graphics::DirectionalLight>;
		using MaterialBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Graphics::Material>;
		using SettingsBuffer = Angazi::Graphics::TypedConstantBuffer<Settings>;
		using DepthMapConstantBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Math::Matrix4>;

		TransformBuffer mTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;

		Settings mSettings;
		TransformData transformData;

		Angazi::Graphics::VertexShader mVertexShader;
		Angazi::Graphics::PixelShader  mPixelShader;
		Angazi::Graphics::Sampler mSampler;
		Angazi::Graphics::BlendState mBlendState;

		Angazi::Graphics::Texture mDiffuseMap;
		Angazi::Graphics::Texture mSpecularMap;
		Angazi::Graphics::Texture mDisplacementMap;
		Angazi::Graphics::Texture mNormalMap;

		//Refractions
		Angazi::Graphics::RenderTarget mRefractionRenderTarget;
		//Reflections
		Angazi::Graphics::RenderTarget mReflectionRenderTarget;
	};
}