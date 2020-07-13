#pragma once

#ifdef ENABLE_DIRECTX11

namespace Angazi::Graphics::TextureUtil
{
	ID3D11ShaderResourceView* CreateCubeMapFromTexture(ID3D11ShaderResourceView* texture
		, const std::filesystem::path & shaderFilePath, uint32_t cubeLength);
}

#endif