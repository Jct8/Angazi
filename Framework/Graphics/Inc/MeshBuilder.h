#pragma once
#include "Mesh.h"

namespace Angazi::Graphics
{
	class MeshBuilder
	{
	public:
		static MeshP CreateInnerCubeP();

		static MeshPC CreateCubePC();
		static MeshPC CreateCylinderPC();

		static MeshPX CreateCubePX();
		static MeshPX CreateCylinderPX(int height, int radius, int sectors = 16);
		static MeshPX CreateNDCQuad();
		static MeshPX CreatePlanePX(int height, int width);
		static MeshPX CreateSpherePX(float radius, int rings = 16, int slices = 16, bool IsInsideOut = false);

		static MeshPN CreateCylinderPN();
		static MeshPN CreateSpherePN(float radius, int rings = 16, int slices = 16);

		static Mesh CreateCube();
		static Mesh CreatePlane(float size, int rings = 16, int slices = 16);
		static Mesh CreateSphere(float radius, int rings = 16, int slices = 16, int repeatedUVS = 1);

		static void ComputeNormals(Mesh &mesh);
	};
}