#include "Precompiled.h"
#include "DepthStencilState.h"

#ifdef ENABLE_DIRECTX11

#include "GraphicsSystemDX11.h"

using namespace Angazi::Graphics;

DepthStencilState::~DepthStencilState()
{
	ASSERT(mDepthStencilState == nullptr, "DepthStencilState -- DepthStencil state not released!");
}

void DepthStencilState::Initialize(bool depthEnable, bool depthWrite)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = depthEnable ? TRUE : FALSE;
	depthStencilDesc.DepthWriteMask = depthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	ASSERT(SUCCEEDED(hr), "DepthStencilState -- Failed to create depth stencil state.");
}

void DepthStencilState::Terminate()
{
	SafeRelease(mDepthStencilState);
}

void DepthStencilState::Set()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetDepthStencilState(mDepthStencilState,0);
}

void DepthStencilState::Clear()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetDepthStencilState(nullptr,0);
}

#endif