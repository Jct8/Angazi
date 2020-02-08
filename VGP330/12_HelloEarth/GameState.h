#pragma once
#include <Angazi/Inc/Angazi.h>

class GameState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;

	void DebugUI() override;

private:
	Angazi::Graphics::Camera mCamera;

	Angazi::Graphics::Mesh mMesh;
	Angazi::Graphics::MeshBuffer mMeshBuffer;

	Angazi::Graphics::DirectionalLight mDirectionalLight;
	Angazi::Graphics::Material mMaterial;

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
		float bumpMapWeight = 1.0f;
		float normalMapWeight = 1.0f;
		float padding;
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

	Angazi::Graphics::VertexShader mPhongShadingVertexShader;
	Angazi::Graphics::PixelShader mPhongShadingPixelShader;

	Angazi::Graphics::VertexShader mCloudShadingVertexShader;
	Angazi::Graphics::PixelShader mCloudShadingPixelShader;

	Angazi::Graphics::Sampler mSampler;
	Angazi::Graphics::Texture mTexture;
	Angazi::Graphics::Texture mSpecularTexture;
	Angazi::Graphics::Texture mDisplacementTexture;
	Angazi::Graphics::Texture mNormalMap;
	Angazi::Graphics::Texture mNightMap;
	Angazi::Graphics::Texture mClouds;

	Angazi::Graphics::Blending mAlphaBlending;

	Angazi::Math::Vector3 mRotation = 0.0f;
};
