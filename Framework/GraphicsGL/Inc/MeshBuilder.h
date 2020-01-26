#pragma once

namespace Angazi::GraphicsGL
{
	class MeshBuilder
	{
	public:
		static MeshPX CreatePlanePX(int height, int width);
		static MeshPX CreateCylinderPX(int height, int radius, int sectors = 16);
		static MeshPX CreateSpherePX(float radius, int rings = 16, int slices = 16, bool IsInsideOut = false);

		static MeshPN CreateSpherePN(float radius, int rings = 16, int slices = 16);
		static Mesh CreateSphere(float radius, int rings = 16, int slices = 16);
	};

}