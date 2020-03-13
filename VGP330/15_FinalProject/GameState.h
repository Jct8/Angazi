#pragma once
#include <Angazi/Inc/Angazi.h>

class GameState : public Angazi::AppState
{
public:
	enum  RenderType
	{
		Reflection,
		Refraction,
		Normal,
	};
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;

	void DebugUI() override;

private:
	void DrawScene(RenderType rendertype);
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

	struct Clipping
	{
		Angazi::Math::Vector4 plane;
	};

	using TransformBuffer = Angazi::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Graphics::DirectionalLight>;
	using MaterialBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Graphics::Material>;
	using SettingsBuffer = Angazi::Graphics::TypedConstantBuffer<Settings>;
	using DepthMapConstantBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Math::Matrix4>;
	using ShadowConstantBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Math::Matrix4>;
	using ClippingConstantBuffer = Angazi::Graphics::TypedConstantBuffer<Clipping>;


	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	SettingsBuffer mSettingsBuffer;

	Settings mSettings;
	Settings mGroundSettings;
	Settings mTankSettings;

	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader  mPixelShader;

	//Textures
	Angazi::Graphics::Sampler mSampler;
	Angazi::Graphics::Texture mSpecularTexture;
	Angazi::Graphics::Texture mDisplacementTexture;
	Angazi::Graphics::Texture mNormalMap;
	Angazi::Graphics::Texture mTexture;
	Angazi::Graphics::Texture mGroundTexture;

	Angazi::Graphics::BlendState mBlendState;
	Angazi::Math::Vector3 mRotation = 0.0f;

	//Clipping
	Clipping mClipping;
	ClippingConstantBuffer mClippingConstantBuffer;

	//Refractions
	Angazi::Graphics::RenderTarget mRefractionRenderTarget;
	Angazi::Graphics::VertexShader mRefractionVertexShader;
	Angazi::Graphics::PixelShader mRefractionPixelShader;
	DepthMapConstantBuffer mRefractionConstantBuffer;

	//Reflections
	Angazi::Graphics::RenderTarget mReflectionRenderTarget;

	//Reflections - Tank
	Angazi::Graphics::Mesh mTankMesh;
	Angazi::Graphics::MeshBuffer mTankMeshBuffer;
	Angazi::Math::Vector3 mTankRotation = 0.0f;
	Angazi::Math::Vector3 mTankPosition;
	Angazi::Graphics::Texture mTankTexture;
	Angazi::Graphics::Texture mTankSpecularTexture;
	Angazi::Graphics::Texture mTankDisplacementTexture;
	Angazi::Graphics::Texture mTankNormalMap;
	Angazi::Graphics::Texture mTankAOMap;

	ShadowConstantBuffer mShadowConstantBuffer;

	//PostProcessing
	Angazi::Graphics::RenderTarget mRenderTarget;
	Angazi::Graphics::MeshPX mScreenQuad;
	Angazi::Graphics::MeshBuffer mScreenQuadBuffer;
	Angazi::Graphics::VertexShader mPostProcessingVertexShader;
	Angazi::Graphics::PixelShader mPostProcessingPixelShader;

};
