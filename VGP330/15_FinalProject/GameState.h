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
	Angazi::Graphics::Material mModelMaterial;

	// Textures
	Angazi::Graphics::Sampler mSampler;;

	Angazi::Math::Vector3 mRotation = 0.0f;
	Angazi::Math::Vector3 mTranslation = 0.0f;
	Angazi::Math::Vector3 mGroundTranslation = { -1.0f,-2.0f,0.0f };

	Angazi::Math::Vector4 mClippingPlane;

	// Reflections - Boat
	Angazi::Graphics::Mesh mBoatMesh;
	Angazi::Graphics::MeshBuffer mBoatMeshBuffer;
	Angazi::Math::Vector3 mBoatRotation = 0.0f;
	Angazi::Math::Vector3 mBoatPosition;

	// Terrain mesh
	Angazi::Graphics::Mesh mRockyTerrain;
	Angazi::Graphics::MeshBuffer mRockyTerrainBuffer;

	// Effects
	Angazi::Graphics::StandardEffect mBoatEffect;
	Angazi::Graphics::PbrEffect mGroundEffect;
	Angazi::Graphics::WaterEffect mWaterEffect;
	Angazi::Graphics::HdrEffect mHdrEffect;

	// PostProcessing
	Angazi::Graphics::RenderTarget mRenderTarget;
	Angazi::Graphics::MeshBuffer mScreenQuadBuffer;
	Angazi::Graphics::VertexShader mPostProcessingVertexShader;
	Angazi::Graphics::PixelShader mPostProcessingPixelShader;

	Angazi::Graphics::Skybox mSkybox;

	float waterBrightness = 1.7f;
	float waterMovementSpeed = 0.020f;
	float waterDisplacement = 0.165f;
	float waterReflectionPower = 0.5f;
};
