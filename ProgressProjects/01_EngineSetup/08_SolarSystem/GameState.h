#pragma once
#include <Angazi/Inc/Angazi.h>

class GameState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;

private:
	Angazi::Graphics::Camera mCamera;

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

	float mRotation = 0.0f;
};