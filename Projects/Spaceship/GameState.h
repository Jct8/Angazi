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
	Angazi::Graphics::DirectionalLight mDirectionalLight;
	Angazi::Graphics::Material mMaterial;

	Angazi::Graphics::MeshPX mMeshSphere;

	Angazi::Graphics::MeshBuffer mMeshBufferSphere;

	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader mPixelShader;
	Angazi::Graphics::ConstantBuffer mConstantBuffer;

	Angazi::Graphics::Sampler mSampler;
	std::deque<Angazi::Graphics::Texture> mPlanetTextures;
	std::deque<Angazi::Graphics::Texture> mMoonTextures;

	std::vector<float> mRotationSpeed;
	std::vector<float> mSelfRotationSpeed;
	std::vector<float> mScale;
	std::vector<float> mTranslation;

	Angazi::Graphics::Skybox mSkybox;

	float mShipRotation = 0.0f;
	float mShipElevation = 0.0f;
	float mShipTilt = 0.0f;
	Angazi::Math::Vector3 mShipPosition;
	Angazi::Math::Vector3 mShipDirection;

	float mCameraDistanceOffset = 3.0f;
	float mCameraHeightOffset = 1.0f;
	Angazi::Math::Vector3 mCameraTargetPosition;


	Angazi::Graphics::Mesh mSpaceshipMesh;
	Angazi::Graphics::MeshBuffer mMeshBufferSpaceship;
	Angazi::Graphics::StandardEffect mStandardEffect;

	std::array<Angazi::Graphics::ParticleEmitter,2> mThrusters;

	float mRotation = 0.0f;
};