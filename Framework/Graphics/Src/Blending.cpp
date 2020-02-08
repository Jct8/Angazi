#include "Precompiled.h"
#include "Blending.h"

#include "D3DUtil.h"

using namespace Angazi::Graphics;

void Blending::Initialize()
{
	D3D11_BLEND_DESC omDesc;
	ZeroMemory(&omDesc, sizeof(D3D11_BLEND_DESC));
	omDesc.RenderTarget[0].BlendEnable = true;
	omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	Graphics::GetDevice()->CreateBlendState(&omDesc, &mD3dBlendState);
}

void Blending::Terminate()
{
	SafeRelease(mD3dBlendState);
}

void Blending::Bind() const
{
	Graphics::GetContext()->OMSetBlendState(mD3dBlendState, 0, 0xffffffff);
}

void Blending::UnBind() const
{
	Graphics::GetContext()->OMSetBlendState(nullptr, 0, 0xffffffff);
}
