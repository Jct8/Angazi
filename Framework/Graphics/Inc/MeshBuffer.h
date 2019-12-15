#pragma once

namespace Angazi::Graphics
{
	class MeshBuffer
	{
	public:
		template <class MeshType>
		void Initialize(const MeshType& mesh)
		{
			Initialize( mesh.vertices.data(), static_cast<int>(mesh.vertices.size()), mesh.indices.data() , static_cast<int>(mesh.indices.size()));
		}

		template <class VertexType>
		void Initialize(const VertexType * vertices, int vertexCount, const  uint32_t* indices, int indexCount)
		{
			InitializeInternal(vertices,sizeof(VertexType), vertexCount, indices,indexCount);
		}


		void Terminate();

		void Draw();
	private:
		ID3D11Buffer* mVertexBuffer = nullptr;
		ID3D11Buffer* mIndexBuffer = nullptr;

		void InitializeInternal(const void * vertices, int vertexSize, int vertexCount, const  uint32_t* indices, int indexCount);
		int mIndexCount = 0;
		int mVertexSize = 0;
	};

}