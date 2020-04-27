#include "Precompiled.h"
#include "MeshIO.h"

using namespace Angazi::Graphics;

void MeshIO::Write(FILE * file, const Mesh & mesh)
{
	// Vertices
	int numVertices = mesh.vertices.size();
	fprintf_s(file, "VertexCount: %d\n", numVertices);
	for (int i = 0; i < numVertices; ++i)
	{
		fprintf_s(file, "p %f %f %f ", mesh.vertices[i].position.x, mesh.vertices[i].position.y, mesh.vertices[i].position.z);
		fprintf_s(file, "n %f %f %f ", mesh.vertices[i].normal.x, mesh.vertices[i].normal.y, mesh.vertices[i].normal.z);
		fprintf_s(file, "t %f %f %f ", mesh.vertices[i].tangent.x, mesh.vertices[i].tangent.y, mesh.vertices[i].tangent.z);
		fprintf_s(file, "tx %f %f\n", mesh.vertices[i].normal.x , mesh.vertices[i].normal.y);
	}

	// Indices
	int numIndices = mesh.indices.size();
	fprintf_s(file, "IndicesCount: %d\n", numIndices);
	for (int i = 0; i < numIndices; ++i)
	{
		fprintf_s(file, "%d ", mesh.indices[i]);
	}
	fprintf_s(file, "\n");
}

void MeshIO::Read(FILE * file, Mesh & mesh)
{
	// Vertices
	int numVertices = 0;
	fscanf_s(file, "VertexCount: %d\n", &numVertices);
	for (int i = 0; i < numVertices; ++i)
	{
		mesh.vertices.emplace_back();
		fscanf_s(file, "p %f %f %f ", &mesh.vertices[i].position.x, &mesh.vertices[i].position.y, &mesh.vertices[i].position.z);
		fscanf_s(file, "n %f %f %f ", &mesh.vertices[i].normal.x,   &mesh.vertices[i].normal.y,   &mesh.vertices[i].normal.z);
		fscanf_s(file, "t %f %f %f ", &mesh.vertices[i].tangent.x,  &mesh.vertices[i].tangent.y,  &mesh.vertices[i].tangent.z);
		fscanf_s(file, "tx %f %f\n"  , &mesh.vertices[i].normal.x,   &mesh.vertices[i].normal.y);
	}

	// Indices
	int numIndices = 0;
	fscanf_s(file, "IndicesCount: %d\n", &numIndices);
	for (int i = 0; i < numIndices; ++i)
	{
		mesh.indices.emplace_back();
		fscanf_s(file, "%d ", &mesh.indices[i]);
	}
	fscanf_s(file, "\n");
}
