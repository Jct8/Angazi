#include "Precompiled.h"
#include "Mesh.h"

#include "MeshBuilder.h"

using namespace Angazi;
using namespace Angazi::Graphics;

MeshPX MeshBuilder::CreatePlanePX(int height, int width)
{
	MeshPX retMesh;
	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			float u = static_cast<float>(x) / static_cast<float>(width);
			float v = static_cast<float>(y) / static_cast<float>(height);
			retMesh.vertices.push_back({ Math::Vector3{static_cast<float>(x),-static_cast<float>(y) , 0.0f } , u , v });

			retMesh.indices.push_back(y * height + x);
			retMesh.indices.push_back((y + 1) * height + x + 1);
			retMesh.indices.push_back((y + 1) * height + x);

			retMesh.indices.push_back(y * height + x);
			retMesh.indices.push_back(y * height + x + 1);
			retMesh.indices.push_back((y + 1) * height + x + 1);

		}
	}

	return retMesh;
}

MeshPX MeshBuilder::CreateCylinderPX(int height, int radius, int sectors)
{
	MeshPX retMesh;
	float increment = Math::Constants::TwoPi / sectors;

	for (int y = 0; y <= height; y++)
	{
		for (float theta = 0; theta <= Math::Constants::TwoPi; theta += increment)
		{
			float u = theta / Math::Constants::TwoPi;
			float v = static_cast<float>(y) / static_cast<float>(height);
			retMesh.vertices.push_back(
				{ Math::Vector3{ cosf(theta) * radius, -static_cast<float>(y) , sinf(theta) * radius }
				, u , v });
		}
	}

	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x < sectors; x++)
		{
			retMesh.indices.push_back(y * height + x);
			retMesh.indices.push_back((y + 1) * height + x + 1);
			retMesh.indices.push_back((y + 1) * height + x);

			retMesh.indices.push_back(y * height + x);
			retMesh.indices.push_back(y * height + x + 1);
			retMesh.indices.push_back((y + 1)* height + x + 1);
		}
	}

	return retMesh;
}

MeshPX MeshBuilder::CreateSpherePX(float radius, int rings , int slices)
{
	MeshPX retMesh;
	float thetaIncrement = Math::Constants::TwoPi / rings;
	float phiIncrement = Math::Constants::Pi / slices;

	for (float phi = 0; phi <= Math::Constants::Pi; phi+= phiIncrement)
	{
		for (float theta = 0; theta <= Math::Constants::TwoPi; theta += thetaIncrement)
		{
			float u = theta / Math::Constants::TwoPi;
			float v = static_cast<float>(phi) / static_cast<float>(Math::Constants::Pi);
			float newRadius = radius * sinf(phi);
			retMesh.vertices.push_back(
				{ Math::Vector3{ newRadius * cosf(theta), radius * cosf(phi) , newRadius * sinf(theta) }
				, u , v });
		}
	}

	for (int y = 0; y <= slices; y++)
	{
		for (int x = 0; x < rings; x++)
		{
			retMesh.indices.push_back(y * slices + x);
			retMesh.indices.push_back((y + 1) * slices + x + 1);
			retMesh.indices.push_back((y + 1) * slices + x);

			retMesh.indices.push_back(y * slices + x);
			retMesh.indices.push_back(y * slices + x + 1);
			retMesh.indices.push_back((y + 1)* slices + x + 1);
		}
	}

	return retMesh;
}
