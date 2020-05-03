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
	void DrawScene();
	void PostProcess();

private:
	Angazi::Graphics::Camera mCamera;

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
		float bumpMapWeight = 0.4f;
		float normalMapWeight = 1.0f;
		float aoMapWeight = 1.0f;
		float brightness = 1.0f;
		int useShadow = 0;
		float depthBias = 0.0003f;
		float padding[1];
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
	Settings mModelsettings;


	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader  mPixelShader;

	Angazi::Graphics::Sampler mSampler;
	Angazi::Graphics::BlendState mBlendState;

	// Jet
	Angazi::Graphics::Mesh mJetMesh;
	Angazi::Graphics::MeshBuffer mJetMeshBuffer;
	Angazi::Math::Vector3 mJetRotation = 0.0f;
	Angazi::Math::Vector3 mJetPosition;
	Angazi::Graphics::Texture mJetTexture;
	Angazi::Graphics::Texture mJetSpecularTexture;
	Angazi::Graphics::Texture mJetDisplacementTexture;
	Angazi::Graphics::Texture mJetNormalMap;
	Angazi::Graphics::Texture mJetAOMap;

	// Model
	Angazi::Graphics::Model model;

	// Animation
	Angazi::Graphics::Animation mAnimation;

	// PostProcessing
	Angazi::Graphics::RenderTarget mRenderTarget;
	Angazi::Graphics::MeshPX mScreenQuad;
	Angazi::Graphics::MeshBuffer mScreenQuadBuffer;
	Angazi::Graphics::VertexShader mPostProcessingVertexShader;
	Angazi::Graphics::PixelShader mPostProcessingPixelShader;

	float dt = 0.0f;
};
