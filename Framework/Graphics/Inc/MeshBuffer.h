#pragma once

namespace Angazi::Graphics
{
	struct Vertex
	{
		Angazi::Math::Vector3 position;
		Angazi::Math::Vector4 color;
		//Angazi::Graphics::Color color;
	};
	struct Mesh
	{
		std::vector<Vertex> mVertices;
		std::vector<uint32_t> mIndices;
	};
	
	class MeshBuffer
	{
	public:
		void Initialize(Vertex * vertices, int vertexCount, uint32_t* indices, int indexCount);
		void Terminate();
		void Draw();
	private:
		int mIndexCount = 0;
		ID3D11Buffer* mVertexBuffer = nullptr;
		ID3D11Buffer* mIndexBuffer = nullptr;
	};

}