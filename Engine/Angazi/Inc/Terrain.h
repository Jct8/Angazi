#pragma once

namespace Angazi
{
	class Terrain
	{
	public:
		void Initialize(uint32_t numRows, uint32_t numCols, float cellSize);
		void Terminate();

		void LoadHeightmap(const std::filesystem::path& filePath);
		void SetHeightScale(float scale);

		void SetDirectionalLight(const Graphics::DirectionalLight& light);
		void SetClippingPlane(const Math::Vector4& plane);
		void Render(const Graphics::Camera& camera);

		void DrawEditorUI();

	private:
		void GenerateVertices();
		void GenerateIndices();

		struct ConstantData
		{
			Math::Matrix4 world;
			Math::Matrix4 wvp;
			Math::Vector3 viewPosition;
			float padding;
			Graphics::DirectionalLight directionalLight;
			Angazi::Math::Vector4 clippingPlane;
		};
		Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;
		Graphics::VertexShader mTerrainVertexShader;
		Graphics::PixelShader mTerrainPixelShader;

		Graphics::Sampler mSampler;
		Graphics::Texture mDiffuseTexture;
		Graphics::Texture mNormalTexture;

		Graphics::Mesh mMesh;
		Graphics::MeshBuffer mMeshBuffer;

		ConstantData mConstantData;

		uint32_t mNumRows = 0;
		uint32_t mNumCols = 0;
		uint32_t mNumCellsInCol = 0;
		uint32_t mNumCellsInRow = 0;
		uint32_t mNumCells = 0;

		float mHeightScale = 1.0f;
		float mCellSize = 1.0f;
	};
}