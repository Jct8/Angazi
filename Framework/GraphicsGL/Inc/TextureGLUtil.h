#pragma once

#ifdef ENABLE_OPENGL

namespace Angazi::Graphics::TextureUtil
{
	uint32_t CreateCubeMapFromTexture(uint32_t texture, const std::filesystem::path& shaderFilePath, uint32_t cubeLength);

	uint32_t CreatePreFilteredCubeMap(uint32_t texture, const std::filesystem::path& shaderFilePath, uint32_t cubeLength);
}

#endif