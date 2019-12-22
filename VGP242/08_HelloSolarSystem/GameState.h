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
	Angazi::Graphics::MeshPX mMeshDomeSphere;

	Angazi::Graphics::MeshBuffer mMeshBufferSphere;
	Angazi::Graphics::MeshBuffer mMeshBufferDome;

	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader mPixelShader;
	Angazi::Graphics::ConstantBuffer mConstantBuffer;

	Angazi::Graphics::Sampler mSampler;
	std::vector<Angazi::Graphics::Texture> mPlanetTextures;
	std::vector<Angazi::Graphics::Texture> mMoonTextures;
	Angazi::Graphics::Texture mDomeTexture;

	std::vector<float> mRotationSpeed;
	std::vector<float> mSelfRotationSpeed;
	std::vector<float> mScale;
	std::vector<float> mTranslation;

	float mRotation = 0.0f;
};