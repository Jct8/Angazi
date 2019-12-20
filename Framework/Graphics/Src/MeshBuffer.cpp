#include "Precompiled.h"
#include "MeshBuffer.h"

#include "D3DUtil.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void MeshBuffer::InitializeInternal(const void * vertices, int vertexSize, int vertexCount,const  uint32_t* indices, int indexCount)
{
	mVertexSize = vertexSize;
	mIndexCount = indexCount;

	//Create vertex buffer
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = vertexCount * vertexSize;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//Initialization
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices;

	//hresult is an error code as a long
	auto device = GetDevice();
	HRESULT	hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");

	//Create index Buffer
	bufferDesc.ByteWidth = indexCount * sizeof(uint32_t);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	initData.pSysMem = indices;

	hr = device->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create index buffer.");
	auto context = GetContext();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void MeshBuffer::Terminate()
{
	SafeRelease(mIndexBuffer);
	SafeRelease(mVertexBuffer);
}

void MeshBuffer::Draw()
{
	auto context = GetContext();

	UINT stride = mVertexSize;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed((UINT)mIndexCount, 0, 0);
}
