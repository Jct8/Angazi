#pragma once

#include "Material.h"

namespace Angazi::Graphics
{
	using MeshId = size_t;

	//struct MaterialInfo
	//{
	//	Material material;
	//	TextureId
	//};

	class MeshLoader
	{
	public:
		static void Load(const std::filesystem::path& filePath, float scale, std::vector<MeshId>& meshes, bool useRootPath = true);
	};
}