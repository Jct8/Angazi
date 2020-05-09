#pragma once
#include "Mesh.h"

namespace Angazi::Graphics::MeshIO
{
	void Write(FILE *file, const SkinnedMesh& mesh);
	void Read(FILE* file, SkinnedMesh& mesh);
}