#include "Precompiled.h"
#ifdef ENABLE_DIRECTX11

#include "ConstantBuffer.h"

#include "D3DUtil.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Math;

ConstantBuffer::~ConstantBuffer()
{
	ASSERT(mConstantBuffer == nullptr, "[ConstantBuffer] Terminate() must be called to clean up!");
}

void ConstantBuffer::Initialize(int size)
{
	//Create constant buffer
	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.ByteWidth = size;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	auto device = GetDevice();
	HRESULT hr = device->CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create constant buffer.");
}

void ConstantBuffer::Terminate()
{
	SafeRelease(mConstantBuffer);
}

void ConstantBuffer::BindVS(uint32_t slot) const
{
	GetContext()->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindPS(uint32_t slot) const
{
	GetContext()->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::UnbindVS(uint32_t slot) const
{
	static ID3D11Buffer* dummy = nullptr;
	GetContext()->VSSetConstantBuffers(slot, 1, &dummy);
}

void ConstantBuffer::UnbindPS(uint32_t slot) const
{
	static ID3D11Buffer* dummy = nullptr;
	GetContext()->PSSetConstantBuffers(slot, 1, &dummy);
}

void ConstantBuffer::Update(const void * data) const
{
	GetContext()->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);
}

#endif