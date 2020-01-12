#pragma once

#include "Colors.h"

namespace Angazi::Graphics { class Camera; } // forward declare

namespace Angazi::Graphics::SimpleDraw {

	void StaticInitialize(uint32_t maxVertexCount = 10000);
	void StaticTerminate();

	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
	//void AddBox(..., const Color& color);
	//void AddSphere(..., const Color& color);

	void Render(const Camera& camera);

} // namespace Angazi::Graphics::SimpleDraw 