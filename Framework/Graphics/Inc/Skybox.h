#pragma once

#include "MeshBuffer.h"
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "VertexTypes.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"

namespace Angazi::Graphics
{
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

		void Draw(Math::Matrix4 wvp);

	private:
		struct TransformData
		{
			Angazi::Math::Matrix4 wvp;
		};
		using TransformBuffer = Angazi::Graphics::TypedConstantBuffer<TransformData>;

		DepthStencilState mDepthStencilState;
		RasterizerState mRasterizerState;

		ID3D11ShaderResourceView *mShaderResourceView = nullptr;
		std::map<Side, std::filesystem::path> cubeSides;

		Angazi::Graphics::VertexShader mVertexShader;
		Angazi::Graphics::PixelShader  mPixelShader;
		Angazi::Graphics::Sampler mSampler;

		TransformBuffer mTransformBuffer;
		TransformData mTransformData;

		MeshBuffer mBoxBuffer;
	};
}