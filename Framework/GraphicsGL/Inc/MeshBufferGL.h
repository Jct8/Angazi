#pragma once
#ifdef ENABLE_OPENGL

namespace Angazi::Graphics
{
	class MeshBuffer
	{
	public:
		enum class Topology
		{
			Lines,
			Triangles,
			TrianglesStrip
		};

		MeshBuffer() = default;
		~MeshBuffer();

		MeshBuffer(const MeshBuffer&) = delete;
		MeshBuffer& operator=(const MeshBuffer&) = delete;

		MeshBuffer(MeshBuffer &&) = default;
		MeshBuffer& operator=(MeshBuffer &&) = default;

		template<class MeshType>
		void Initialize(const MeshType &mesh, bool dynamic = false)
		{
			Initialize(mesh.vertices.data(), static_cast<int>(mesh.vertices.size()), mesh.indices.data(), static_cast<int>(mesh.indices.size()),dynamic);
		}
		
		template<class VertexType>
		void Initialize(const VertexType * vertices, int vertexCount,bool dynamic = false)
		{
			InitializeInternal(vertices, sizeof(VertexType), vertexCount, nullptr, 0 , VertexType::Format, dynamic);
		}

		template<class VertexType>
		void Initialize(const VertexType * vertices, int vertexCount, const uint32_t* indices, int indexCount, bool dynamic = false)
		{
			InitializeInternal(vertices, sizeof(VertexType), vertexCount, indices, indexCount, VertexType::Format, dynamic);
		}

		void SetTopology(Topology topology);
		void Update(const void* VertexData, uint32_t numVertices);
		
		void Terminate();
		void Draw() const;

	private:
		void InitializeInternal(const void * vertices, int vertexSize, int vertexCount, const uint32_t* indices, int indexCount, uint32_t vertextFormat, bool dynamic = false);

		unsigned int mVertexBuffer;
		unsigned int mIndexBuffer;
		unsigned int mVertexArray;
		unsigned int mTopology = GL_TRIANGLES;

		int mVertexCount = 0;
		int mVertexSize = 0;
		int mIndexCount = 0;
	};

} //namespace Angazi::GraphicsGL

#endif