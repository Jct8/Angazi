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

	Angazi::Graphics::MeshBuffer mPlaneMeshBuffer;

	Angazi::Graphics::DirectionalLight mDirectionalLight;
	Angazi::Graphics::Material mMaterial;

	//Terrain
	Angazi::Terrain mTerrain;

	//Textures
	Angazi::Graphics::Sampler mSampler;;
	Angazi::Graphics::BlendState mBlendState;

	Angazi::Math::Vector3 mRotation = 0.0f;
	Angazi::Math::Vector3 mTranslation = 0.0f;
	Angazi::Math::Vector3 mGroundTranslation = { -1.0f,-2.0f,0.0f };

	Angazi::Math::Vector4 mClippingPlane;

	//Reflections - Tank
	Angazi::Graphics::Mesh mTankMesh;
	Angazi::Graphics::MeshBuffer mTankMeshBuffer;
	Angazi::Math::Vector3 mTankRotation = 0.0f;
	Angazi::Math::Vector3 mTankPosition;

	// Effects
	Angazi::Graphics::StandardEffect mTankEffect;
	Angazi::Graphics::StandardEffect mGroundEffect;
	Angazi::Graphics::WaterEffect mWaterEffect;

	// PostProcessing
	Angazi::Graphics::RenderTarget mRenderTarget;
	Angazi::Graphics::MeshBuffer mScreenQuadBuffer;
	Angazi::Graphics::VertexShader mPostProcessingVertexShader;
	Angazi::Graphics::PixelShader mPostProcessingPixelShader;

	Angazi::Graphics::Skybox mSkybox;

	float waterBrightness = 1.7f;
	float waterMovementSpeed = 0.020f;
	float waterDisplacement = 0.165f;
};
