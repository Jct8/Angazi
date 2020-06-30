#pragma once

namespace Angazi::Graphics
{
	class Camera;

	class Skybox
	{
	public:
		enum Side
		{
			Right,
			Left,
			Top,
			Bottom,
			Front,
			Back
		};
	public:
		Skybox();
		~Skybox();

		void AddTexture(const std::filesystem::path& path, Side side);
		void CreateSkybox();
		void Terminate();

		void ChangeDefualtSkybox(int skyboxNumber);
		void Draw(const Angazi::Graphics::Camera& camera);
		Angazi::Graphics::Texture* GetSkyboxTexture() { return &mTexture; };
	private:
		struct TransformData
		{
			Angazi::Math::Matrix4 wvp;
		};
		using TransformBuffer = Angazi::Graphics::TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;
		TransformData mTransformData;

		Angazi::Graphics::DepthStencilState mDepthStencilState;
		Angazi::Graphics::RasterizerState mRasterizerState;

		Angazi::Graphics::MeshBuffer mBoxBuffer;

		Angazi::Graphics::VertexShader mVertexShader;
		Angazi::Graphics::PixelShader  mPixelShader;
		Angazi::Graphics::Sampler mSampler;
		Angazi::Graphics::Texture mTexture;

		std::map<Side,std::filesystem::path> cubeSides;
	};
}