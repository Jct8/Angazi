#include "Precompiled.h"
#include "D3DUtil.h"

#include "GraphicsSystem.h"

ID3D11Device * Angazi::Graphics::GetDevice()
{
	return GraphicsSystem::Get()->mD3DDevice;
}

ID3D11DeviceContext * Angazi::Graphics::GetContext()
{
	return GraphicsSystem::Get()->mImmediateContext;
}
