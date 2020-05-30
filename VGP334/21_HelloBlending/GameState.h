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
	Angazi::Graphics::StandardEffect mArenaStandardEffect;
	Angazi::Graphics::StandardEffect mGroundStandardEffect;

	// Model
	Angazi::Graphics::Model model;
	Angazi::Math::Vector3 mModelPosition = 0.0f;
	Angazi::Graphics::Animator animator;
	float mMovementSpeed = 10.0f;

	//Camera
	Angazi::Math::Vector3 mCameraOffset;

	//Input
	Angazi::Math::Vector2 mInputAxis;

	//Ground
	Angazi::Graphics::Mesh mGroundMesh;
	Angazi::Graphics::MeshBuffer mGroundMeshBuffer;

	//Shadow
	Angazi::Graphics::ShadowEffect mShadowEffect;

	// PostProcessing
	Angazi::Graphics::PostProcessingEffect mPostProcessingEffect;


	// Skeleton - Animation
	float animationSpeed = 1.0f;
	bool mShowSkeleton = false;

};
