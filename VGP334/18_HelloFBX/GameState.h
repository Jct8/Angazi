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

	// Jet
	Angazi::Graphics::Mesh mJetMesh;
	Angazi::Graphics::MeshBuffer mJetMeshBuffer;
	Angazi::Math::Vector3 mJetRotation = 0.0f;
	Angazi::Math::Vector3 mJetPosition;

	// Standard Effects
	Angazi::Graphics::StandardEffect mJetStandardEffect;
	Angazi::Graphics::StandardEffect mModelStandardEffect;
	Angazi::Graphics::StandardEffect mGroundStandardEffect;
	Angazi::Graphics::HdrEffect mHdrEffect;

	// Model
	Angazi::Graphics::Model model;

	// Animation
	Angazi::Graphics::Animation mAnimation;

	//Ground
	Angazi::Graphics::Mesh mGroundMesh;
	Angazi::Graphics::MeshBuffer mGroundMeshBuffer;

	//Shadow
	Angazi::Graphics::ShadowEffect mShadowEffect;

	// PostProcessing
	Angazi::Graphics::PostProcessingEffect mPostProcessingEffect;

	float dt = 0.0f;

	// Skeleton
	bool mShowSkeleton = false;
	std::vector<Angazi::Math::Matrix4> mBoneMatrices;

};
