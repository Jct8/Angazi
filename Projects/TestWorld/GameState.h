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
	void DrawDepthMap();

private:
	Angazi::Graphics::Camera mCamera;
	Angazi::Graphics::DirectionalLight mDirectionalLight;
	Angazi::Graphics::Material mMaterial;
	Angazi::Graphics::Sampler mSampler;


	// Standard Effects
	Angazi::Graphics::StandardEffect mGroundStandardEffect;
	Angazi::Graphics::HdrEffect mHdrEffect;

	//Ground
	Angazi::Graphics::MeshBuffer mGroundMeshBuffer;

	Angazi::Graphics::Skybox mSkybox;

	// Testing
	Angazi::Graphics::RenderTarget renderTarget;
	Angazi::Graphics::VertexShader vertexShader;
	Angazi::Graphics::PixelShader pixelShader;
	Angazi::Graphics::MeshBuffer meshBuffer;
	Angazi::Graphics::TypedConstantBuffer<Angazi::Math::Matrix4> tranformBuffer;
	Angazi::Graphics::Camera camera;
};
