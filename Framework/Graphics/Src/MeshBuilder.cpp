#include "Precompiled.h"
#include "Mesh.h"

#include "MeshBuilder.h"
#include "DirectXTK/Inc/GeometricPrimitive.h"

//using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Math;

MeshP MeshBuilder::CreateInnerCubeP()
{
	MeshP mesh;
	// Front Vertices
	mesh.vertices.push_back({ Vector3{ -1.0f,  1.0f, -1.0f } });// 0
	mesh.vertices.push_back({ Vector3{  1.0f,  1.0f, -1.0f } });// 1
	mesh.vertices.push_back({ Vector3{  1.0f, -1.0f, -1.0f } });// 2
	mesh.vertices.push_back({ Vector3{ -1.0f, -1.0f, -1.0f } });// 3
	// Back
	mesh.vertices.push_back({ Vector3{ -1.0f,  1.0f, 1.0f } });	// 4
	mesh.vertices.push_back({ Vector3{  1.0f,  1.0f, 1.0f } });	// 5
	mesh.vertices.push_back({ Vector3{  1.0f, -1.0f, 1.0f } });	// 6
	mesh.vertices.push_back({ Vector3{ -1.0f, -1.0f, 1.0f } });	// 7

	// Front
	mesh.indices.push_back(0);
	mesh.indices.push_back(2);
	mesh.indices.push_back(1);

	mesh.indices.push_back(0);
	mesh.indices.push_back(3);
	mesh.indices.push_back(2);

	// Back
	mesh.indices.push_back(4);
	mesh.indices.push_back(5);
	mesh.indices.push_back(6);

	mesh.indices.push_back(4);
	mesh.indices.push_back(6);
	mesh.indices.push_back(7);

	// Right
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);
	mesh.indices.push_back(5);

	mesh.indices.push_back(5);
	mesh.indices.push_back(2);
	mesh.indices.push_back(6);

	// Left
	mesh.indices.push_back(4);
	mesh.indices.push_back(7);
	mesh.indices.push_back(0);

	mesh.indices.push_back(0);
	mesh.indices.push_back(7);
	mesh.indices.push_back(3);

	// Top
	mesh.indices.push_back(4);
	mesh.indices.push_back(0);
	mesh.indices.push_back(1);

	mesh.indices.push_back(4);
	mesh.indices.push_back(1);
	mesh.indices.push_back(5);

	// Bottom
	mesh.indices.push_back(7);
	mesh.indices.push_back(6);
	mesh.indices.push_back(3);

	mesh.indices.push_back(6);
	mesh.indices.push_back(2);
	mesh.indices.push_back(3);
	return mesh;
}

MeshPX MeshBuilder::CreateCubePX()
{
	MeshPX mesh;
	//Front Vertices
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, -0.5f } , {0.0f,0.0f} });// 0
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, -0.5f } , {1.0f,0.0f} });// 1
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, -0.5f } , {1.0f,1.0f} });// 2
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, -0.5f } , {0.0f,1.0f} });// 3
	//Back Vertices
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f,  0.5f } , {1.0f,0.0f} });// 4
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f,  0.5f } , {0.0f,0.0f} });// 5
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f,  0.5f } , {0.0f,1.0f} });// 6
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f,  0.5f } , {1.0f,1.0f} });// 7
	//Left Vertices
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f,  0.5f } , {0.0f,0.0f} });// 8
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, -0.5f } , {1.0f,0.0f} });// 9
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, -0.5f } , {1.0f,1.0f} });// 10
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f,  0.5f } , {0.0f,1.0f} });// 11
	//Right Vertices
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, -0.5f } , {0.0f,0.0f} });// 12
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f,  0.5f } , {1.0f,0.0f} });// 13
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f,  0.5f } , {1.0f,1.0f} });// 14
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, -0.5f } , {0.0f,1.0f} });// 15
	// Top Vertices
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f,  0.5f } , {0.0f,0.0f} });// 16
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f,  0.5f } , {1.0f,0.0f} });// 17
	mesh.vertices.push_back({ Vector3{  0.5f,  0.5f, -0.5f } , {1.0f,1.0f} });// 18
	mesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, -0.5f } , {0.0f,1.0f} });// 19
	// Bottom Vertices
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, -0.5f } , {0.0f,0.0f} });// 20
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f, -0.5f } , {1.0f,0.0f} });// 21
	mesh.vertices.push_back({ Vector3{  0.5f, -0.5f,  0.5f } , {1.0f,1.0f} });// 22
	mesh.vertices.push_back({ Vector3{ -0.5f, -0.5f,  0.5f } , {0.0f,1.0f} });// 23

	// Front Indices
	mesh.indices.push_back(0); mesh.indices.push_back(1); mesh.indices.push_back(2);
	mesh.indices.push_back(0); mesh.indices.push_back(2); mesh.indices.push_back(3);

	// Back Indices
	mesh.indices.push_back(5); mesh.indices.push_back(4); mesh.indices.push_back(7);
	mesh.indices.push_back(5); mesh.indices.push_back(7); mesh.indices.push_back(6);

	// Right Indices
	mesh.indices.push_back(12); mesh.indices.push_back(13); mesh.indices.push_back(14);
	mesh.indices.push_back(12); mesh.indices.push_back(14); mesh.indices.push_back(15);

	// Left Indices
	mesh.indices.push_back(8); mesh.indices.push_back(9);  mesh.indices.push_back(10);
	mesh.indices.push_back(8); mesh.indices.push_back(10); mesh.indices.push_back(11);

	// Top Indices
	mesh.indices.push_back(16); mesh.indices.push_back(17); mesh.indices.push_back(18);
	mesh.indices.push_back(16); mesh.indices.push_back(18); mesh.indices.push_back(19);

	// Bottom Indices
	mesh.indices.push_back(20); mesh.indices.push_back(21); mesh.indices.push_back(23);
	mesh.indices.push_back(21); mesh.indices.push_back(22); mesh.indices.push_back(23);

	return mesh;
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

Mesh MeshBuilder::CreateCube()
{
	Mesh mesh;
	Math::Vector3 position;
	//Front Vertices
	mesh.vertices.push_back({ { -0.5f,  0.5f, -0.5f }, {0.0f,0.0f,-1.0f}, {1.0f,0.0f,0.0f} , {0.0f,0.0f} });// 0
	mesh.vertices.push_back({ {  0.5f,  0.5f, -0.5f }, {0.0f,0.0f,-1.0f}, {1.0f,0.0f,0.0f} , {1.0f,0.0f} });// 1
	mesh.vertices.push_back({ {  0.5f, -0.5f, -0.5f }, {0.0f,0.0f,-1.0f}, {1.0f,0.0f,0.0f} , {1.0f,1.0f} });// 2
	mesh.vertices.push_back({ { -0.5f, -0.5f, -0.5f }, {0.0f,0.0f,-1.0f}, {1.0f,0.0f,0.0f} , {0.0f,1.0f} });// 3
	//Back Vertices
	mesh.vertices.push_back({ { -0.5f,  0.5f,  0.5f }, {0.0f,0.0f, 1.0f}, {-1.0f,0.0f,0.0f} , {0.0f,0.0f} });// 4
	mesh.vertices.push_back({ {  0.5f,  0.5f,  0.5f }, {0.0f,0.0f, 1.0f}, {-1.0f,0.0f,0.0f} , {1.0f,0.0f} });// 5
	mesh.vertices.push_back({ {  0.5f, -0.5f,  0.5f }, {0.0f,0.0f, 1.0f}, {-1.0f,0.0f,0.0f} , {1.0f,1.0f} });// 6
	mesh.vertices.push_back({ { -0.5f, -0.5f,  0.5f }, {0.0f,0.0f, 1.0f}, {-1.0f,0.0f,0.0f} , {0.0f,1.0f} });// 7
	//Left Vertices
	mesh.vertices.push_back({ { -0.5f,  0.5f,  0.5f }, {-1.0f,0.0f,0.0f}, {0.0f,0.0f,1.0f} , {0.0f,0.0f} });// 8
	mesh.vertices.push_back({ { -0.5f,  0.5f, -0.5f }, {-1.0f,0.0f,0.0f}, {0.0f,0.0f,1.0f} , {1.0f,0.0f} });// 9
	mesh.vertices.push_back({ { -0.5f, -0.5f, -0.5f }, {-1.0f,0.0f,0.0f}, {0.0f,0.0f,1.0f} , {1.0f,1.0f} });// 10
	mesh.vertices.push_back({ { -0.5f, -0.5f,  0.5f }, {-1.0f,0.0f,0.0f}, {0.0f,0.0f,1.0f} , {0.0f,1.0f} });// 11
	//Right Vertices
	mesh.vertices.push_back({ {  0.5f,  0.5f, -0.5f }, {1.0f,0.0f,0.0f}, {0.0f,0.0f,-1.0f} , {0.0f,0.0f} });// 12
	mesh.vertices.push_back({ {  0.5f,  0.5f,  0.5f }, {1.0f,0.0f,0.0f}, {0.0f,0.0f,-1.0f} , {1.0f,0.0f} });// 13
	mesh.vertices.push_back({ {  0.5f, -0.5f,  0.5f }, {1.0f,0.0f,0.0f}, {0.0f,0.0f,-1.0f} , {1.0f,1.0f} });// 14
	mesh.vertices.push_back({ {  0.5f, -0.5f, -0.5f }, {1.0f,0.0f,0.0f}, {0.0f,0.0f,-1.0f} , {0.0f,1.0f} });// 15
	// Top Vertices
	mesh.vertices.push_back({ { -0.5f,  0.5f,  0.5f }, {0.0f,1.0f,0.0f}, {1.0f,0.0f,0.0f} , {0.0f,0.0f} });// 16
	mesh.vertices.push_back({ {  0.5f,  0.5f,  0.5f }, {0.0f,1.0f,0.0f}, {1.0f,0.0f,0.0f} , {1.0f,0.0f} });// 17
	mesh.vertices.push_back({ {  0.5f,  0.5f, -0.5f }, {0.0f,1.0f,0.0f}, {1.0f,0.0f,0.0f} , {1.0f,1.0f} });// 18
	mesh.vertices.push_back({ { -0.5f,  0.5f, -0.5f }, {0.0f,1.0f,0.0f}, {1.0f,0.0f,0.0f} , {0.0f,1.0f} });// 19
	// Bottom Vertices
	mesh.vertices.push_back({ { -0.5f, -0.5f, -0.5f }, {0.0f,-1.0f,0.0f}, {-1.0f,0.0f,0.0f} , {0.0f,0.0f} });// 20
	mesh.vertices.push_back({ {  0.5f, -0.5f, -0.5f }, {0.0f,-1.0f,0.0f}, {-1.0f,0.0f,0.0f} , {1.0f,0.0f} });// 21
	mesh.vertices.push_back({ {  0.5f, -0.5f,  0.5f }, {0.0f,-1.0f,0.0f}, {-1.0f,0.0f,0.0f} , {1.0f,1.0f} });// 22
	mesh.vertices.push_back({ { -0.5f, -0.5f,  0.5f }, {0.0f,-1.0f,0.0f}, {-1.0f,0.0f,0.0f} , {0.0f,1.0f} });// 23
	// Front Indices
	mesh.indices.push_back(0); mesh.indices.push_back(1); mesh.indices.push_back(2);
	mesh.indices.push_back(2); mesh.indices.push_back(3); mesh.indices.push_back(0);

	// Back Indices
	mesh.indices.push_back(5); mesh.indices.push_back(4); mesh.indices.push_back(7);
	mesh.indices.push_back(5); mesh.indices.push_back(7); mesh.indices.push_back(6);

	// Right Indices
	mesh.indices.push_back(12); mesh.indices.push_back(13); mesh.indices.push_back(14);
	mesh.indices.push_back(12); mesh.indices.push_back(14); mesh.indices.push_back(15);

	// Left Indices
	mesh.indices.push_back(8); mesh.indices.push_back(9);  mesh.indices.push_back(10);
	mesh.indices.push_back(8); mesh.indices.push_back(10); mesh.indices.push_back(11);

	// Top Indices
	mesh.indices.push_back(16); mesh.indices.push_back(17); mesh.indices.push_back(18);
	mesh.indices.push_back(16); mesh.indices.push_back(18); mesh.indices.push_back(19);

	// Bottom Indices
	mesh.indices.push_back(20); mesh.indices.push_back(21); mesh.indices.push_back(23);
	mesh.indices.push_back(21); mesh.indices.push_back(22); mesh.indices.push_back(23);

	return mesh;
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

void MeshBuilder::ComputeNormals(Mesh &mesh)
{
	// get 3 indices
	// get verties, get 2 vectors, cross product to get normal
	// create normal array of size vertices
	// add calculated normal to normal array slot
	// once complete normalize all noramls in array then add back to slot

	std::vector<Angazi::Math::Vector3> newNormals;
	newNormals.reserve(mesh.vertices.size());
	for (size_t i = 0; i < mesh.vertices.size(); ++i)
		newNormals.push_back({ 0.0f, 0.0f ,0.0f });
	for (size_t i = 0; i < mesh.indices.size(); i += 3)
	{
		auto vector1 = mesh.vertices[mesh.indices[i]].position - mesh.vertices[mesh.indices[i + 1]].position;
		auto vector2 = mesh.vertices[mesh.indices[i]].position - mesh.vertices[mesh.indices[i + 2]].position;

		Angazi::Math::Vector3 normal = Angazi::Math::Cross(vector1, vector2);
		newNormals[mesh.indices[i]] += normal;
		newNormals[mesh.indices[i + 1]] += normal;
		newNormals[mesh.indices[i + 2]] += normal;
	}
	for (size_t i = 0; i < newNormals.size(); ++i)
		mesh.vertices[i].normal = Angazi::Math::Normalize(newNormals[i]);

}