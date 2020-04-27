#pragma once
#include "Mesh.h"
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

		template <class MeshType>
		void Initialize(const MeshType& mesh, bool dynamic = false)
		{
			Initialize( mesh.vertices.data(), static_cast<int>(mesh.vertices.size()), mesh.indices.data() , static_cast<int>(mesh.indices.size()), dynamic);
		}

		template <class VertexType>
		void Initialize(const VertexType * vertices, int vertexCount, bool dynamic = false)
		{
			InitializeInternal(vertices, sizeof(VertexType), vertexCount, nullptr, 0, dynamic);
		}

		template <class VertexType>
		void Initialize(const VertexType * vertices, int vertexCount, const  uint32_t* indices, int indexCount,bool dynamic = false)
		{
			InitializeInternal(vertices,sizeof(VertexType), vertexCount, indices,indexCount, dynamic);
		}

		void SetTopology(Topology topology);
		void Update(const void* vertexData, uint32_t numVertices);

		static void ComputeNormals(Mesh &mesh);

		void Terminate();
		void Draw() const;
	private:
		ID3D11Buffer* mVertexBuffer = nullptr;
		ID3D11Buffer* mIndexBuffer = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		void InitializeInternal(const void * vertices, int vertexSize, int vertexCount, const  uint32_t* indices, int indexCount, bool dynamic);
		int mVertexCount = 0;
		int mVertexSize = 0;
		int mIndexCount = 0;
	};

}