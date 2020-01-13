#pragma once

namespace Angazi::GraphicsGL
{
	class MeshBufferGL
	{
	public:
		enum class Topology
		{
			Lines,
			Triangles
		};

		template<class MeshType>
		void Initialize(const MeshType &mesh, uint32_t vertextFormat, bool dynamic = false)
		{
			Initialize(mesh.vertices.data(), static_cast<int>(mesh.vertices.size()), mesh.indices.data(), static_cast<int>(mesh.indices.size()),vertextFormat,dynamic);
		}
		
		template<class VertexType>
		void Initialize(const VertexType * vertices, int vertexCount, uint32_t vertextFormat,bool dynamic = false)
		{
			InitializeInternal(vertices, sizeof(VertexType), vertexCount, nullptr, 0 , vertextFormat, dynamic);
		}

		template<class VertexType>
		void Initialize(const VertexType * vertices, int vertexCount, const uint32_t* indices, int indexCount, uint32_t vertextFormat, bool dynamic = false)
		{
			InitializeInternal(vertices, sizeof(VertexType), vertexCount, indices, indexCount,vertextFormat, dynamic);
		}

		void SetTopology(Topology topology);
		void Update(const void* VertexData, uint32_t numVertices);
		
		void Terminate();
		void Draw();

	private:
		void InitializeInternal(const void * vertices, int vertexSize, int vertexCount, const uint32_t* indices, int indexCount, uint32_t vertextFormat, bool dynamic = false);

		unsigned int mVertexBuffer;
		unsigned int mIndexBuffer;
		unsigned int mVertexArray;
		unsigned int mTopology = GL_TRIANGLES;

		int mIndexCount;
		int mVertexSize;
		int mVertexCount;
	};

} //namespace Angazi::GraphicsGL