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
		static MeshPX CreatePlanePX(int height, int width);
		static MeshPX CreateCylinderPX(int height, int radius, int sectors = 16);
		static MeshPX CreateSpherePX(float radius, int rings = 16, int slices = 16, bool IsInsideOut = false);
		static MeshPX CreateNDCQuad();

		static MeshPN CreateCylinderPN();
		static MeshPN CreateSpherePN(float radius, int rings = 16, int slices = 16);

		static Mesh CreateSphere(float radius, int rings = 16, int slices = 16);
		static Mesh CreatePlane(float size, int rings = 16, int slices = 16);

		static void ComputeNormals(Mesh &mesh);
	};
}