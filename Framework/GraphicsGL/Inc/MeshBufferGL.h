#pragma once

namespace Angazi::Graphics
{
	class MeshBufferGL
	{
	public:
		template<class MeshType>
		void Initialize(const MeshType &mesh, uint32_t vertextFormat)
		{
			Initialize(mesh.vertices.data(), static_cast<int>(mesh.vertices.size()), mesh.indices.data(), static_cast<int>(mesh.indices.size()),vertextFormat);
		}
		
		template<class VertexType>
		void Initialize(const VertexType * vertices, int vertexCount, const uint32_t* indices, int indexCount, uint32_t vertextFormat)
		{
			InitializeInternal(vertices, sizeof(VertexType), vertexCount, indices, indexCount,vertextFormat);
		}
		
		void Terminate();
		void Draw();

	private:
		void InitializeInternal(const void * vertices, int vertexSize, int vertexCount, const uint32_t* indices, int indexCount, uint32_t vertextFormat);

		unsigned int mVertexBuffer;
		unsigned int mIndexBuffer;
		unsigned int mVertexArray;

		int mIndexCount;
		int mVertexSize;
	};

} //namespace Angazi::Graphics