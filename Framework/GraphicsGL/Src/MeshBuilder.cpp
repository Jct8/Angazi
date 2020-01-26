#include "Precompiled.h"
#include "Mesh.h"
#include "MeshBuilder.h"

using namespace Angazi;
using namespace Angazi::GraphicsGL;

MeshPX MeshBuilder::CreatePlanePX(int height, int width)
{
	MeshPX retMesh;
	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			float u = static_cast<float>(x) / static_cast<float>(width);
			float v = static_cast<float>(y) / static_cast<float>(height);
			retMesh.vertices.push_back({
				Math::Vector3{-0.5f*width + static_cast<float>(x) ,  0.5f*height - static_cast<float>(y)  , 0.0f } , Math::Vector2{u , v} });

			if (x != width)
			{
				retMesh.indices.push_back(y * height + x);
				retMesh.indices.push_back((y + 1) * height + x + 1);
				retMesh.indices.push_back((y + 1) * height + x);

				retMesh.indices.push_back(y * height + x);
				retMesh.indices.push_back(y * height + x + 1);
				retMesh.indices.push_back((y + 1) * height + x + 1);
			}
		}
	}
	return retMesh;
}

MeshPX MeshBuilder::CreateCylinderPX(int height, int radius, int sectors)
{
	MeshPX retMesh;
	float increment = Math::Constants::TwoPi / static_cast<float>(sectors);

	for (int y = 0; y <= height; y++)
	{
		for (float theta = 0; theta <= Math::Constants::TwoPi; theta += increment)
		{
			float u = theta / Math::Constants::TwoPi;
			float v = static_cast<float>(y) / static_cast<float>(height);
			retMesh.vertices.push_back(
				{ Math::Vector3{ cosf(theta) * radius, 0.5f*height - y , sinf(theta) * radius }
				, Math::Vector2{u , v } });
		}
	}

	int sectorCount = sectors + 1;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < sectors; x++)
		{
			retMesh.indices.push_back(y * sectorCount + x);
			retMesh.indices.push_back((y + 1) * sectorCount + x + 1);
			retMesh.indices.push_back((y + 1) * sectorCount + x);

			retMesh.indices.push_back(y * sectorCount + x);
			retMesh.indices.push_back(y * sectorCount + x + 1);
			retMesh.indices.push_back((y + 1)* sectorCount + x + 1);
		}
	}

	return retMesh;
}

MeshPX MeshBuilder::CreateSpherePX(float radius, int rings, int slices, bool IsInsideOut)
{
	MeshPX retMesh;
	float phiIncrement = Math::Constants::Pi / (rings);
	float thetaIncrement = Math::Constants::TwoPi / (slices);
	float phi = 0.0f;
	float theta = 0.0f;

	for (int i = 0; i <= rings; i++)
	{
		float v = static_cast<float>(i) / rings;
		float theta = 0.0f;
		for (int j = 0; j <= slices; j++)
		{
			float u = static_cast<float>(j) / (slices);
			float newRadius = radius * sinf(phi);
			Math::Vector3 vec = Math::Vector3{ newRadius* sinf(theta), radius * cosf(phi) , newRadius * -cosf(theta) };
			retMesh.vertices.push_back(
				{ vec , Math::Vector2{u , v } });
			theta += thetaIncrement;
		}
		phi += phiIncrement;
	}

	for (int y = 0; y <= rings; y++)
	{
		for (int x = 0; x <= slices; x++)
		{
			if (IsInsideOut)
			{
				retMesh.indices.push_back((y + 1) * slices + x);
				retMesh.indices.push_back((y + 1) * slices + x + 1);
				retMesh.indices.push_back(y * slices + x);

				retMesh.indices.push_back((y + 1)* slices + x + 1);
				retMesh.indices.push_back(y * slices + x + 1);
				retMesh.indices.push_back(y * slices + x);
			}
			else
			{
				retMesh.indices.push_back(y * slices + x);
				retMesh.indices.push_back((y + 1) * slices + x + 1);
				retMesh.indices.push_back((y + 1) * slices + x);

				retMesh.indices.push_back(y * slices + x);
				retMesh.indices.push_back(y * slices + x + 1);
				retMesh.indices.push_back((y + 1)* slices + x + 1);
			}
		}
	}

	return retMesh;
}

MeshPN MeshBuilder::CreateSpherePN(float radius, int rings, int slices)
{
	MeshPN retMesh;
	float phiIncrement = Math::Constants::Pi / rings;
	float thetaIncrement = Math::Constants::TwoPi / slices;
	float phi = 0.0f;
	float theta = 0.0f;

	for (int i = 0; i <= rings; i++)
	{
		float v = static_cast<float>(i) / rings;
		float theta = 0.0f;
		for (int j = 0; j <= slices; j++)
		{
			float u = static_cast<float>(j) / (slices);
			float newRadius = radius * sinf(phi);
			Math::Vector3 vec = Math::Vector3{ newRadius* sinf(theta), radius * cosf(phi) , newRadius * -cosf(theta) };
			retMesh.vertices.push_back(
				{ vec, Math::Normalize(vec) });
			theta += thetaIncrement;
		}
		phi += phiIncrement;
	}

	for (int y = 0; y <= rings; y++)
	{
		for (int x = 0; x <= slices; x++)
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

Mesh MeshBuilder::CreateSphere(float radius, int rings, int slices)
{
	Mesh retMesh;
	float phiIncrement = Math::Constants::Pi / rings;
	float thetaIncrement = Math::Constants::TwoPi / slices;
	float phi = 0.0f;
	float theta = 0.0f;

	for (int i = 0; i <= rings; i++)
	{
		float v = static_cast<float>(i) / rings;
		float theta = 0.0f;
		for (int j = 0; j <= slices; j++)
		{
			float u = static_cast<float>(j) / (slices);
			float newRadius = radius * sinf(phi);
			Math::Vector3 vec = Math::Vector3{ newRadius* sinf(theta), radius * cosf(phi) , newRadius * -cosf(theta) };
			retMesh.vertices.push_back
			(
				{ vec, Math::Normalize(vec) , vec , Math::Vector2{u,v} }
			);
			theta += thetaIncrement;
		}
		phi += phiIncrement;
	}

	for (int y = 0; y <= rings; y++)
	{
		for (int x = 0; x <= slices; x++)
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