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
	Angazi::Graphics::Skybox mSkybox;

	Angazi::Graphics::DirectionalLight mDirectionalLight;
	Angazi::Graphics::Material mMaterial;
	Angazi::Graphics::Sampler mSampler;

	Angazi::Graphics::MeshBuffer mMeshBufferSphere;

	Angazi::Graphics::Texture mPlainTexture;
	Angazi::Graphics::PbrEffect mPbrEffect;

	Angazi::Math::Vector3 mRotation = 0.0f;
	bool useTextureMap = false;
};
