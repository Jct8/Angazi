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

private:
	Angazi::Graphics::Camera mCamera;

	Angazi::Graphics::DirectionalLight mDirectionalLight;
	Angazi::Graphics::Material mMaterial;

	Angazi::Graphics::Sampler mSampler;

	// Standard Effects
	Angazi::Graphics::StandardEffect mModelStandardEffect;
	Angazi::Graphics::StandardEffect mGroundStandardEffect;

	// Model
	Angazi::Graphics::Model model;
	Angazi::Graphics::Animator animator;

	// Ground
	Angazi::Graphics::Mesh mGroundMesh;
	Angazi::Graphics::MeshBuffer mGroundMeshBuffer;

	// Shadow
	Angazi::Graphics::ShadowEffect mShadowEffect;

	// PostProcessing
	Angazi::Graphics::PostProcessingEffect mPostProcessingEffect;

	// Skybox
	Angazi::Graphics::Skybox mSkybox;

	// Skeleton - Animation
	float animationSpeed = 10.0f;
	bool mShowSkeleton = false;
};
