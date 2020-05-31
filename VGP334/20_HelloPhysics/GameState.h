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
	void DrawScene();
	void PostProcess();

private:
	Angazi::Graphics::Camera mCamera;

	// Physics
	Angazi::Physics::PhysicsWorld mPhysicsWorld;
	std::vector<Angazi::Physics::Particle*> mParticles;

	// Texture Cloth 
	Angazi::Graphics::Sampler mSampler;
	Angazi::Graphics::Texture mTexture;
	Angazi::Graphics::MeshPX mMesh;
	Angazi::Graphics::MeshBuffer mMeshBuffer;
	Angazi::Graphics::PixelShader mPixelShader;
	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::ConstantBuffer mConstantBuffer;

	bool usingClothTexture = false;
	int height = 20;
	int width = 15;
};
