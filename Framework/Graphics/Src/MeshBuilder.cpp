#include "Precompiled.h"
#include "Mesh.h"

#include "MeshBuilder.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Math;

MeshPX MeshBuilder::CreateCubePX()
{
	MeshPX mMesh;
	//Front Vertices
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.0f } , {0.0f,0.0f} });// 0
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.0f } , {1.0f,0.0f} });// 1
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f } , {1.0f,1.0f} });// 2
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } , {0.0f,1.0f} });// 3
	//Back Vertices
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 1.0f } , {1.0f,0.0f} });// 4
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 1.0f } , {0.0f,0.0f} });// 5
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 1.0f } , {0.0f,1.0f} });// 6
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 1.0f } , {1.0f,1.0f} });// 7
	//Left Vertices
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 1.0f } , {0.0f,0.0f} });// 8
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.0f } , {1.0f,0.0f} });// 9
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } , {1.0f,1.0f} });// 10
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 1.0f } , {0.0f,1.0f} });// 11
	//Right Vertices
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.0f } , {0.0f,0.0f} });// 12
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 1.0f } , {1.0f,0.0f} });// 13
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 1.0f } , {1.0f,1.0f} });// 14
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f } , {0.0f,1.0f} });// 15
	// Top Vertices
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 1.0f } , {0.0f,0.0f} });// 16
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 1.0f } , {1.0f,0.0f} });// 17
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.0f } , {1.0f,1.0f} });// 18
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.0f } , {0.0f,1.0f} });// 19
	// Bottom Vertices
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } , {0.0f,0.0f} });// 20
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f } , {1.0f,0.0f} });// 21
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 1.0f } , {1.0f,1.0f} });// 22
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 1.0f } , {0.0f,1.0f} });// 23

	// Front Indices
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(2);

	mMesh.indices.push_back(0);
	mMesh.indices.push_back(2);
	mMesh.indices.push_back(3);

	// Back Indices
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(7);

	mMesh.indices.push_back(5);
	mMesh.indices.push_back(7);
	mMesh.indices.push_back(6);

	// Right Indices
	mMesh.indices.push_back(12);
	mMesh.indices.push_back(13);
	mMesh.indices.push_back(14);

	mMesh.indices.push_back(12);
	mMesh.indices.push_back(14);
	mMesh.indices.push_back(15);

	// Left Indices
	mMesh.indices.push_back(8);
	mMesh.indices.push_back(9);
	mMesh.indices.push_back(10);

	mMesh.indices.push_back(8);
	mMesh.indices.push_back(10);
	mMesh.indices.push_back(11);

	// Top Indices
	mMesh.indices.push_back(16);
	mMesh.indices.push_back(17);
	mMesh.indices.push_back(18);

	mMesh.indices.push_back(16);
	mMesh.indices.push_back(18);
	mMesh.indices.push_back(19);

	// Bottom Indices
	mMesh.indices.push_back(20);
	mMesh.indices.push_back(21);
	mMesh.indices.push_back(23);

	mMesh.indices.push_back(21);
	mMesh.indices.push_back(22);
	mMesh.indices.push_back(23);

	return mMesh;
}

MeshPX MeshBuilder::CreatePlanePX(int height, int width)
{
	MeshPX retMesh;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float u = static_cast<float>(x) / static_cast<float>(width-1);
			float v = static_cast<float>(y) / static_cast<float>(height-1);
			retMesh.vertices.push_back({
				Math::Vector3{-0.5f*width + static_cast<float>(x) ,  0.5f*height - static_cast<float>(y)  , 0.0f } , Math::Vector2{u , v} });

			if ((x != width - 1) && (y!= height-1))
			{
				retMesh.indices.push_back(y * width + x);
				retMesh.indices.push_back((y + 1) * width + x + 1);
				retMesh.indices.push_back((y + 1) * width + x);

				retMesh.indices.push_back(y * width + x);
				retMesh.indices.push_back(y * width + x + 1);
				retMesh.indices.push_back((y + 1) * width + x + 1);
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
			Math::Vector3 normalized = Math::Normalize(vec);

			retMesh.vertices.push_back
			(
				{ vec, normalized , Math::Vector3{-normalized.z , 0.0f , normalized.x} , Math::Vector2{u,v} }
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

Mesh MeshBuilder::CreatePlane(float size, int height, int width)
{
	/*Mesh retMesh;
	float xIncrement = static_cast<float>(size/ (rows-1));
	float zIncrement = static_cast<float>(size/ (columns-1 ));
	float x = 0.0f;
	float z = 0.0f;

	for (int i = 0; i <= rows; i++)
	{
		float v = static_cast<float>(i) / rows;
		float x = 0.0f;
		for (int j = 0; j <= columns; j++)
		{
			float u = static_cast<float>(j) / (columns);
			Math::Vector3 normal = Math::Vector3::YAxis;
			Math::Vector3 tangent = Math::Vector3::ZAxis;
			Math::Vector3 vec = Math::Vector3{ x, 0.0f ,z };

			retMesh.vertices.push_back
			(
				{ vec, normal , tangent , Math::Vector2{u,v} }
			);
			z += zIncrement;
		}
		x += xIncrement;
	}

	for (int y = 0; y <= rows; y++)
	{
		for (int x = 0; x <= columns; x++)
		{
			retMesh.indices.push_back(y * columns + x);
			retMesh.indices.push_back((y + 1) * columns + x + 1);
			retMesh.indices.push_back((y + 1) * columns + x);

			retMesh.indices.push_back(y * columns + x);
			retMesh.indices.push_back(y * columns + x + 1);
			retMesh.indices.push_back((y + 1)* columns + x + 1);

		}
	}
	return retMesh;*/

	Mesh retMesh;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float u = static_cast<float>(x) / static_cast<float>(width-1);
			float v = static_cast<float>(y) / static_cast<float>(height-1);
			Math::Vector3 normal = Math::Vector3::YAxis;
			Math::Vector3 tangent = Math::Vector3::ZAxis;
			retMesh.vertices.push_back({
				Math::Vector3{-0.5f*width + static_cast<float>(x) , 0.0f ,  0.5f*height - static_cast<float>(y)} , normal,tangent , Math::Vector2{u , v} });

			if ((x != width - 1) && (y != height - 1))
			{
				retMesh.indices.push_back(y * width + x);
				retMesh.indices.push_back((y + 1) * width + x + 1);
				retMesh.indices.push_back((y + 1) * width + x);

				retMesh.indices.push_back(y * width + x);
				retMesh.indices.push_back(y * width + x + 1);
				retMesh.indices.push_back((y + 1) * width + x + 1);
			}
		}
	}
	return retMesh;
}

MeshPX MeshBuilder::CreateNDCQuad()
{
	VertexPX vertices[] =
	{
		{{-1.0f,1.0f,0.0f}, {0.0f,0.0f}},
		{ { 1.0f,1.0f,0.0f }, {1.0f,0.0f} },
		{ { 1.0f,-1.0f,0.0f }, {1.0f,1.0f} },
		{ { -1.0f,-1.0f,0.0f }, {0.0f,1.0f} }
	};
	uint32_t indices[] =
	{
		0,1,2,0,2,3
	};

	MeshPX mesh;
	mesh.vertices.insert(mesh.vertices.end(), std::begin(vertices), std::end(vertices));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
	/*MeshPX retMesh;
	int height = 2;
	int width = 2;
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
	return retMesh;*/
}

//Mesh MeshBuilder::CreateSphere(float radius, int rings, int slices)
//{
//	Mesh retMesh;
//	float phiIncrement = Math::Constants::Pi / rings;
//	float thetaIncrement = Math::Constants::TwoPi / slices;
//
//	for (float phi = 0; phi <= Math::Constants::Pi ; phi += phiIncrement)
//	{
//		for (float theta = 0; theta <= Math::Constants::TwoPi; theta += thetaIncrement)
//		{
//			float u = theta / Math::Constants::TwoPi;
//			float v = static_cast<float>(phi) / static_cast<float>(Math::Constants::Pi);
//			float newRadius = radius * sinf(phi);
//			Math::Vector3 vec = Math::Vector3{ newRadius* -sinf(theta), radius * cosf(phi) , newRadius * cosf(theta) };
//			retMesh.vertices.push_back
//			(
//				{ vec, Math::Normalize(vec) ,Math::Normalize(vec), Math::Vector2{u,v} }
//			);
//		}
//	}
//
//	for (int y = 0; y <= rings; y++)
//	{
//		for (int x = 0; x <= slices; x++)
//		{
//			/*auto base = x + (y*rings+y);
//			retMesh.indices.push_back(base);
//			retMesh.indices.push_back(base+rings+2);
//			retMesh.indices.push_back(base+rings+1);
//
//			retMesh.indices.push_back(base);
//			retMesh.indices.push_back(base + 1);
//			retMesh.indices.push_back(base + rings + 2);*/
//
//			/*retMesh.indices.push_back(y * slices + x);
//			retMesh.indices.push_back((y + 1) * slices + x + 2);
//			retMesh.indices.push_back((y + 1)  * slices + x + 1);
//
//			retMesh.indices.push_back(y * slices + x);
//			retMesh.indices.push_back(y * slices + x + 1);
//			retMesh.indices.push_back((y + 1)* slices + x + 2);*/
//
//			/*retMesh.indices.push_back(y * slices + x);
//			retMesh.indices.push_back((y + 1) * slices + x + 1);
//			retMesh.indices.push_back((y + 1) * slices + x);
//
//			retMesh.indices.push_back(y * slices + x);
//			retMesh.indices.push_back(y * slices + x + 1);
//			retMesh.indices.push_back((y + 1)* slices + x + 1);*/
//
//		}
//	}
//	return retMesh;
//}