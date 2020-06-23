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
	TransformData data;
	TransformBuffer mTransformBuffer;

	Angazi::Graphics::Camera mCamera;
	Angazi::Graphics::MeshBuffer mMeshBufferSphere;
	Angazi::Graphics::MeshBuffer mMeshBufferDome;

	Angazi::Graphics::PixelShader mPixelShader;
	Angazi::Graphics::VertexShader mVertexShader;

	std::vector<Angazi::Graphics::Texture> mPlanetTextures;
	std::vector<Angazi::Graphics::Texture> mMoonTextures;
	Angazi::Graphics::Texture mDomeTexture;

	std::vector<float> mRotationSpeed;
	std::vector<float> mSelfRotationSpeed;
	std::vector<float> mScale;
	std::vector<float> mTranslation;

	float mRotation = 0.0f;
};