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
	Angazi::GraphicsGL::CameraGL mCamera;

	Angazi::GraphicsGL::MeshPX mMeshSphere;
	Angazi::GraphicsGL::MeshPX mMeshDomeSphere;

	Angazi::GraphicsGL::MeshBufferGL mMeshBufferSphere;
	Angazi::GraphicsGL::MeshBufferGL mMeshBufferDome;

	Angazi::GraphicsGL::Shader mShader;

	std::vector<Angazi::GraphicsGL::TextureGL> mPlanetTextures;
	std::vector<Angazi::GraphicsGL::TextureGL> mMoonTextures;
	Angazi::GraphicsGL::TextureGL mDomeTexture;

	std::vector<float> mRotationSpeed;
	std::vector<float> mSelfRotationSpeed;
	std::vector<float> mScale;
	std::vector<float> mTranslation;

	float mRotation = 0.0f;
};