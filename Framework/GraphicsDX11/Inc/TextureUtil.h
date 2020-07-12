#pragma once

#ifdef ENABLE_DIRECTX11

namespace Angazi::Graphics::TextureUtil
{
	ID3D11ShaderResourceView* CreateCubeTextureFromHDR(const std::filesystem::path & filePath);
}

#endif