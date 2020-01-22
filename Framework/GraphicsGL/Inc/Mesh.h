#pragma once

#include "VertexTypes.h"

namespace Angazi::GraphicsGL
{
	template<class VertexType>
	struct MeshBase
	{
		std::vector<VertexType> vertices;
		std::vector<uint32_t> indices;
	};

	using MeshPC = MeshBase<VertexPC>;
	using MeshPX = MeshBase<VertexPX>;
	using MeshPN = MeshBase<VertexPN>;

}//namespace Angazi::GraphicsGL