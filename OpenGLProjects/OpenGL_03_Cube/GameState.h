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
	struct TransformData
	{
		Angazi::Math::Matrix4 wvp;
	};
	using TransformBuffer = Angazi::Graphics::TypedConstantBuffer<TransformData>;

	Angazi::Graphics::Camera mCamera;
	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader mPixelShader;
	Angazi::Graphics::Texture mTexture;

	TransformData data;
	TransformBuffer mTransformBuffer;

	Angazi::Graphics::MeshBuffer mMeshBuffer;
	Angazi::Graphics::MeshPX mMesh;

	float mRotation = 0.0f;

};