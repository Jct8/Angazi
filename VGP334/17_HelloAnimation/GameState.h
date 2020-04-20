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
		float bumpMapWeight = 0.4f;
		float normalMapWeight = 1.0f;
		float aoMapWeight = 1.0f;
		float brightness = 1.0f;
		int useShadow = 1;
		float depthBias = 0.0003f;
		float movement = 0.0f;
		float movementSpeed = 0.005f;
		float padding[3];
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
	Settings mGroundSettings;
	Settings mTankSettings;

	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader  mPixelShader;

	// Ground Textures
	Angazi::Graphics::Sampler mSampler;
	Angazi::Graphics::Texture mGroundTexture;
	Angazi::Graphics::BlendState mBlendState;
	Angazi::Math::Vector3 mGrounddTranslation = { -1.0f,-2.0f,0.0f };
	Angazi::Math::Vector3 mRotation = 0.0f;

	//Tank
	Angazi::Graphics::Mesh mTankMesh;
	Angazi::Graphics::MeshBuffer mTankMeshBuffer;
	Angazi::Math::Vector3 mTankRotation = 0.0f;
	Angazi::Math::Vector3 mTankPosition;
	Angazi::Graphics::Texture mTankTexture;
	Angazi::Graphics::Texture mTankSpecularTexture;
	Angazi::Graphics::Texture mTankDisplacementTexture;
	Angazi::Graphics::Texture mTankNormalMap;
	Angazi::Graphics::Texture mTankAOMap;

	//Animation
	Angazi::Graphics::Animation mAnimation;

	//PostProcessing
	Angazi::Graphics::RenderTarget mRenderTarget;
	Angazi::Graphics::MeshPX mScreenQuad;
	Angazi::Graphics::MeshBuffer mScreenQuadBuffer;
	Angazi::Graphics::VertexShader mPostProcessingVertexShader;
	Angazi::Graphics::PixelShader mPostProcessingPixelShader;

	float dt = 0.0f;
};
