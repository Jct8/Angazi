#include "Precompiled.h"
#include "MeshIO.h"

using namespace Angazi::Graphics;

void MeshIO::Write(FILE * file, const Mesh & mesh)
{
	// Vertices
	uint32_t numVertices = static_cast<uint32_t>(mesh.vertices.size());
	fprintf_s(file, "VertexCount: %d\n", numVertices);
	for (uint32_t i = 0; i < numVertices; ++i)
	{
		fprintf_s(file, "p %f %f %f ", mesh.vertices[i].position.x, mesh.vertices[i].position.y, mesh.vertices[i].position.z);
		fprintf_s(file, "n %f %f %f ", mesh.vertices[i].normal.x, mesh.vertices[i].normal.y, mesh.vertices[i].normal.z);
		fprintf_s(file, "t %f %f %f ", mesh.vertices[i].tangent.x, mesh.vertices[i].tangent.y, mesh.vertices[i].tangent.z);
		fprintf_s(file, "tx %f %f\n", mesh.vertices[i].texcoord.x , mesh.vertices[i].texcoord.y);
	}

	// Indices
	uint32_t numIndices = static_cast<uint32_t>(mesh.indices.size());
	fprintf_s(file, "IndexCount: %d\n", numIndices);
	for (uint32_t i = 0; i < numIndices; i += 3)
	{
		fprintf_s(file, "%i %i %i\n", mesh.indices[i], mesh.indices[i + 1], mesh.indices[i + 2]);
	}
}

void MeshIO::Read(FILE * file, Mesh & mesh)
{
	// Vertices
	uint32_t numVertices = 0;
	fscanf_s(file, "VertexCount: %d\n", &numVertices);
	mesh.vertices.resize(numVertices);
	for (uint32_t i = 0; i < numVertices; ++i)
	{
		fscanf_s(file, "p %f %f %f ", &mesh.vertices[i].position.x, &mesh.vertices[i].position.y, &mesh.vertices[i].position.z);
		fscanf_s(file, "n %f %f %f ", &mesh.vertices[i].normal.x,   &mesh.vertices[i].normal.y,   &mesh.vertices[i].normal.z);
		fscanf_s(file, "t %f %f %f ", &mesh.vertices[i].tangent.x,  &mesh.vertices[i].tangent.y,  &mesh.vertices[i].tangent.z);
		fscanf_s(file, "tx %f %f\n"  , &mesh.vertices[i].texcoord.x,   &mesh.vertices[i].texcoord.y);
	}

	// Indices
	uint32_t numIndices = 0;
	fscanf_s(file, "IndexCount: %d\n", &numIndices);
	mesh.indices.resize(numIndices);
	for (uint32_t i = 0; i < numIndices; i += 3)
	{
		uint32_t a, b, c;
		fscanf_s(file, "%i %i %i\n", &a, &b , &c);
		mesh.indices[i] = a;
		mesh.indices[i + 1] = b;
		mesh.indices[i + 2] = c;
	}
}
