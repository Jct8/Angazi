#pragma once

#include "VertexTypes.h"

namespace Angazi::Graphics
{
	template<class VertexType>
	struct MeshBase
	{
		std::vector<VertexType> vertices;
		std::vector<uint32_t> indices;
	};

	using MeshP = MeshBase<VertexP>;
	using MeshPC = MeshBase<VertexPC>;
	using MeshPX = MeshBase<VertexPX>;
	using MeshPN = MeshBase<VertexPN>;
	using Mesh = MeshBase<Vertex>;
	using SkinnedMesh = MeshBase<BoneVertex>;
}