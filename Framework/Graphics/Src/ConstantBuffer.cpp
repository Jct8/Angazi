#include "Precompiled.h"
#include "ConstantBuffer.h"

#include "D3DUtil.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Math;

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

void ConstantBuffer::Bind()
{
	auto context = GetContext();
	context->VSSetConstantBuffers(0, 1, &mConstantBuffer);
}

void ConstantBuffer::Set(void * data)
{
	auto context = GetContext();
	context->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);
}
