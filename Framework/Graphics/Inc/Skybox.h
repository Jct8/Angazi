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
		void CreateSkybox(const std::filesystem::path& hdrImagePath = "None");
		void Terminate();

		void ChangeDefualtSkybox(int skyboxNumber);
		void Draw(const Angazi::Graphics::Camera& camera);

		const Angazi::Graphics::Texture* GetSkyboxTexture() const { return &mTexture; };
		const Angazi::Graphics::Texture* GetIrradianceMap() const { return &mIrradianceMap; };
		const Angazi::Graphics::Texture* GetPrefilteredMap()const { return &mPrefilterMap; };
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
		Angazi::Graphics::Texture mIrradianceMap;
		Angazi::Graphics::Texture mPrefilterMap;

		std::map<Side,std::filesystem::path> cubeSides;
	};
}