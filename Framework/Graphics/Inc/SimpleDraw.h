#pragma once

#include "Colors.h"

namespace Angazi::Graphics { class Camera; } // forward declare

namespace Angazi::Graphics::SimpleDraw {

	void StaticInitialize(uint32_t maxVertexCount = 10000);
	void StaticTerminate();

	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
	void AddLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color);
	void AddBox(float width, float length, float height, const Color & color);
	void AddCone(float height, float radius, const Color& color , int slices = 16);
	void AddSphere(float radius, const Color& color, int slices = 16, int rings = 16);

	void Render(const Camera& camera);

} // namespace Angazi::Graphics::SimpleDraw 