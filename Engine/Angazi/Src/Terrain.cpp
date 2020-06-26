#include "Precompiled.h"
#include "Terrain.h"

using namespace Angazi;

void Terrain::Initialize(uint32_t numRows, uint32_t numCols, float cellSize)
{
	// rows = 3, columns = 4
	// +---+---+---+
	// + / + / + / +
	// +---+---+---+
	// + / + / + / +
	// +---+---+---+

	mNumRows = numRows;
	mNumCols = numCols;
	mNumCellsInCol = mNumRows - 1;
	mNumCellsInRow = mNumCols - 1;
	mNumCells = mNumCellsInCol * mNumCellsInRow;
	mCellSize = cellSize;

	mConstantBuffer.Initialize();

#ifdef ENABLE_DIRECTX11
	mTerrainVertexShader.Initialize("../../Assets/Shaders/Terrain.fx", Graphics::Vertex::Format);
	mTerrainPixelShader.Initialize("../../Assets/Shaders/Terrain.fx");
#endif
#ifdef ENABLE_OPENGL
	mTerrainVertexShader.Initialize("../../Assets/GLShaders/Terrain.glsl", Graphics::Vertex::Format);
	mTerrainPixelShader.Initialize("../../Assets/GLShaders/Terrain.glsl");
#endif

	mSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mGrassTexture.Initialize("../../Assets/Images/grass_2048.jpg");

	GenerateIndices();
	GenerateVertices();

	mMeshBuffer.Initialize(mMesh, true);
}

void Terrain::Terminate()
{
	mMeshBuffer.Terminate();
	mGrassTexture.Terminate();
	mSampler.Terminate();

	mTerrainPixelShader.Terminate();
	mTerrainVertexShader.Terminate();
	mConstantBuffer.Terminate();

}

void Terrain::LoadHeightmap(const std::filesystem::path & filePath)
{
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "rb");

	fseek(file, 0L, SEEK_END);
	uint32_t fileSize = ftell(file);
	uint32_t dimension = (uint32_t)sqrtf((float)fileSize);
	fseek(file, 0L, SEEK_SET);

	for (uint32_t z = 0; z < mNumRows && z < dimension; ++z)
	{
		for (uint32_t x = 0; x < mNumCols && x < dimension; ++x)
		{
			float height = fgetc(file) / 255.0f * mHeightScale;
			mMesh.vertices[x + ((mNumCols - z - 1) * mNumCols)].position.y = height;
		}
	}
	fclose(file);

	Graphics::MeshBuilder::ComputeNormals(mMesh);
	mMeshBuffer.Update(mMesh.vertices.data(), static_cast<uint32_t>(mMesh.vertices.size()));
}
// get 3 indices
// get verties, get 2 vectors, cross product to get normal
// create normal array of size vertices
// add calculated normal to normal array slot
// once complete normalize all noramls in array then add back to slot

void Terrain::SetHeightScale(float scale)
{
	mHeightScale = scale;
}

void Terrain::SetDirectionalLight(const Graphics::DirectionalLight & light)
{
	mConstantData.directionalLight = light;
}

void Terrain::SetClippingPlane(const Math::Vector4 & plane)
{
	mConstantData.clippingPlane = plane;
}

void Terrain::Render(const Graphics::Camera & camera)
{
	auto world = Math::Matrix4::Scaling(0.3f);
	//auto world = Math::Matrix4::Identity;
	auto view = camera.GetViewMatrix();
	auto projection = camera.GetPerspectiveMatrix();


	mConstantData.world = Math::Transpose(world);
	mConstantData.wvp = Math::Transpose(world* view * projection);
	mConstantData.viewPosition = camera.GetPosition();
	mConstantBuffer.Update(&mConstantData);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mSampler.BindPS();
	mGrassTexture.BindPS(0);

	mTerrainVertexShader.Bind();
	mTerrainPixelShader.Bind();

	mMeshBuffer.Draw();

	/*for (int i = 0; i < mMesh.vertices.size(); i++)
	{
		Angazi::Graphics::SimpleDraw::AddLine(mMesh.vertices[i].position, mMesh.vertices[i].position + mMesh.vertices[i].normal, Graphics::Colors::AliceBlue);
	}*/


}

void Terrain::DrawEditorUI()
{
}

void Terrain::GenerateVertices()
{
	mMesh.vertices.clear();
	mMesh.vertices.reserve(mNumRows*mNumCols);

	for (uint32_t z = 0; z < mNumRows; z++)
	{
		for (uint32_t x = 0; x < mNumCols; x++)
		{
			auto& vertex = mMesh.vertices.emplace_back();
			vertex.position = { x *mCellSize, 0.0f, z * mCellSize };
			vertex.normal = { 0.0f, 1.0f, 0.0f };
			vertex.tangent = { 1.0f, 0.0f, 0.0f };
			vertex.texcoord = { (float)x / mNumCellsInCol , 1.0f - ((float)z / mNumCellsInRow) };
		}
	}
}

void Terrain::GenerateIndices()
{
	mMesh.indices.clear();
	mMesh.indices.reserve(mNumCells * 6);

	for (uint32_t z = 0; z < mNumCellsInRow; z++)
	{
		for (uint32_t x = 0; x < mNumCellsInCol; x++)
		{
			// lt --- rt
			// | \     |
			// |   \   |
			// |     \ |
			// lb --- rb

			uint32_t lb = x + (z		 *	mNumCols);
			uint32_t lt = x + ((z + 1) *	mNumCols);
			uint32_t rb = (x + 1) + (z		 *	mNumCols);
			uint32_t rt = (x + 1) + ((z + 1)	 *	mNumCols);

			mMesh.indices.push_back(lb);
			mMesh.indices.push_back(lt);
			mMesh.indices.push_back(rb);

			mMesh.indices.push_back(lt);
			mMesh.indices.push_back(rt);
			mMesh.indices.push_back(rb);

		}
	}
}
