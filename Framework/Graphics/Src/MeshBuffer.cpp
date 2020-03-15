#include "Precompiled.h"
#include "Mesh.h"
#include "MeshBuffer.h"

#include "D3DUtil.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void MeshBuffer::InitializeInternal(const void * vertices, int vertexSize, int vertexCount, const  uint32_t* indices, int indexCount, bool dynamic)
{
	mVertexSize = vertexSize;
	mVertexCount = vertexCount;
	mIndexCount = indexCount;

	//Create vertex buffer
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = vertexCount * vertexSize;
	bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//Initialization
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices;

	//hresult is an error code as a long
	auto device = GetDevice();
	HRESULT	hr = device->CreateBuffer(&bufferDesc, vertices ? &initData : nullptr, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");

	if (indices != nullptr && indexCount > 0)
	{
		//Create index Buffer
		bufferDesc.ByteWidth = indexCount * sizeof(uint32_t);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		initData.pSysMem = indices;

		hr = device->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
		ASSERT(SUCCEEDED(hr), "Failed to create index buffer.");
	}
	//auto context = GetContext();
	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

MeshBuffer::~MeshBuffer()
{
	ASSERT(mVertexBuffer == nullptr, "[MeshBuffer] Terminate() must be called to clean up!");
	ASSERT(mIndexBuffer == nullptr, "[MeshBuffer] Terminate() must be called to clean up!");
}

void MeshBuffer::Terminate()
{
	SafeRelease(mIndexBuffer);
	SafeRelease(mVertexBuffer);
}

void MeshBuffer::SetTopology(Topology topology)
{
	if (topology == Topology::Lines)
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	else if (topology == Topology::Triangles)
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	else if (topology == Topology::TrianglesStrip)
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
}

void MeshBuffer::Update(const void * vertexData, uint32_t numVertices)
{
	mVertexCount = numVertices;

	auto context = GetContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertexData, numVertices*mVertexSize);
	context->Unmap(mVertexBuffer, 0);
}

void MeshBuffer::ComputeNormals(Mesh &mesh)
{
	// get 3 indices
	// get verties, get 2 vectors, cross product to get normal
	// create normal array of size vertices
	// add calculated normal to normal array slot
	// once complete normalize all noramls in array then add back to slot

	std::vector<Angazi::Math::Vector3> newNormals;
	newNormals.reserve(mesh.vertices.size());
	for (int i = 0; i < mesh.vertices.size(); ++i)
		newNormals.push_back({ 0.0f, 0.0f ,0.0f });
	for (int i = 0; i < mesh.indices.size(); i += 3)
	{
		auto vector1 = mesh.vertices[mesh.indices[i]].position - mesh.vertices[mesh.indices[i + 1]].position;
		auto vector2 = mesh.vertices[mesh.indices[i]].position - mesh.vertices[mesh.indices[i + 2]].position;

		Angazi::Math::Vector3 normal = Angazi::Math::Cross(vector1, vector2);
		newNormals[mesh.indices[i]] += normal;
		newNormals[mesh.indices[i+1]] += normal;
		newNormals[mesh.indices[i+2]] += normal;
	}
	for (int i = 0; i < newNormals.size(); ++i)
		mesh.vertices[i].normal = Angazi::Math::Normalize(newNormals[i]);

}

void MeshBuffer::Draw()
{
	auto context = GetContext();

	context->IASetPrimitiveTopology(mTopology);

	UINT stride = mVertexSize;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	if (mIndexBuffer)
	{
		context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed((UINT)mIndexCount, 0, 0);
	}
	else
	{
		context->Draw(mVertexCount, 0);
	}

}
