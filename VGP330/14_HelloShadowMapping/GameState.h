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
	void DrawDepthMap();
	void PostProcess();

private:
	Angazi::Graphics::Camera mDefaultCamera;
	Angazi::Graphics::Camera mDebugCamera;
	Angazi::Graphics::Camera mLightCamera;
	Angazi::Graphics::Camera* mActiveCamera = nullptr;

	std::vector<Angazi::Math::Vector3> mViewFrustumVertices;
	Angazi::Math::Matrix4 mLightProjectionMatrix;

	Angazi::Graphics::Mesh mTankMesh;
	Angazi::Graphics::MeshBuffer mTankMeshBuffer;

	Angazi::Graphics::Mesh mGroundMesh;
	Angazi::Graphics::MeshBuffer mGroundMeshBuffer;

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
		float aoMapWeight = 1.0f;
		float brightness = 1.0f;
		int useShadow = 1;
		float depthBias = 0.0003f;
		float padding[1];
	};

	using TransformBuffer = Angazi::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Graphics::DirectionalLight>;
	using MaterialBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Graphics::Material>;
	using SettingsBuffer = Angazi::Graphics::TypedConstantBuffer<Settings>;
	using DepthMapConstantBuffer = Angazi::Graphics::TypedConstantBuffer<Angazi::Math::Matrix4>;
	using ShadowConstantBuffer  = Angazi::Graphics::TypedConstantBuffer<Angazi::Math::Matrix4>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	SettingsBuffer mSettingsBuffer;

	Settings mSettings;
	Settings mGroundSettings;

	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader mPixelShader;

	Angazi::Graphics::Sampler mSampler;
	Angazi::Graphics::Texture mTexture;
	Angazi::Graphics::Texture mSpecularTexture;
	Angazi::Graphics::Texture mDisplacementTexture;
	Angazi::Graphics::Texture mNormalMap;
	Angazi::Graphics::Texture mAOMap;

	Angazi::Graphics::Texture mGroundTexture;

	Angazi::Graphics::BlendState mBlendState;

	Angazi::Math::Vector3 mTankRotation = 0.0f;
	std::vector<Angazi::Math::Vector3> mTankPositions;
	float mTankSpacing = 20.0f;

	//Shadow
	Angazi::Graphics::RenderTarget mDepthMapRenderTarget;
	Angazi::Graphics::VertexShader mDepthMapVertexShader;
	Angazi::Graphics::PixelShader mDepthMapPixelShader;
	DepthMapConstantBuffer mDepthMapConstantBuffer;
	ShadowConstantBuffer mShadowConstantBuffer;

	//PostProcessing
	Angazi::Graphics::RenderTarget mRenderTarget;
	Angazi::Graphics::MeshPX mScreenQuad;
	Angazi::Graphics::MeshBuffer mScreenQuadBuffer;
	Angazi::Graphics::VertexShader mPostProcessingVertexShader;
	Angazi::Graphics::PixelShader mPostProcessingPixelShader;

};
