#pragma once

#include "Mesh.h"

namespace Angazi::Graphics::MeshIO
{
	void Write(FILE *file, const Mesh& mesh);
	void Read(FILE* file, Mesh& mesh);
}

// For homework: Add MeshIO.cpp and implement Write/Read