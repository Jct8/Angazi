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

	Angazi::Graphics::MeshPX mMesh;

	Angazi::Graphics::MeshBuffer mMeshBuffer;

	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader mPixelShader;
	Angazi::Graphics::ConstantBuffer mConstantBuffer;

	Angazi::Graphics::Sampler mSampler;
	Angazi::Graphics::Texture mTexture;

	float mRotation = 0.0f;
};
