#pragma once
#include "Mesh.h"

namespace Angazi::Graphics
{
	class ObjLoader
	{
	public:
		static void Load(const std::filesystem::path& filePath, float scale, Mesh& mesh);
	};
}