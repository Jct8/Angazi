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
	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader mPixelShader;
	Angazi::Graphics::Texture mTexture;

	Angazi::Graphics::MeshBuffer mMeshBuffer;
	Angazi::Graphics::MeshPX mMesh;

	float mRotation = 0.0f;

};