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
	void SetPBRTextures(std::string textureName);
	void SetStandardTextures(std::string textureName);

	void DrawScene();

private:
	Angazi::Graphics::Camera mCamera;
	Angazi::Graphics::Skybox mSkybox;

	Angazi::Graphics::DirectionalLight mDirectionalLight;
	Angazi::Graphics::Material mMaterial;
	Angazi::Graphics::Sampler mSampler;


	Angazi::Graphics::Mesh mMesh;
	Angazi::Graphics::MeshBuffer mMeshBufferSphere;

	Angazi::Graphics::PbrEffect mPbrEffect;
	Angazi::Graphics::StandardEffect mStandardEffect;
	Angazi::Graphics::HdrEffect mHdrEffect;

	Angazi::Graphics::Texture mPlainTexture;
	std::unordered_map<std::string ,std::unordered_map<std::string,std::unique_ptr<Angazi::Graphics::Texture>>> mTextures;

	Angazi::Math::Vector3 mRotation = 0.0f;
	bool useTextureMap = true;
};
