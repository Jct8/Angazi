#pragma once
#include <Angazi/Inc/Angazi.h>

class GameState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltatTime) override;
	void Render() override;

private:
	Angazi::Graphics::Camera mCamera;
	Angazi::Graphics::MeshPC mMesh;

	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader mPixelShader;
	Angazi::Graphics::ConstantBuffer mConstantBuffer;
	Angazi::Graphics::MeshBuffer mMeshBuffer;

	float mRotation = 0.0f;
};
