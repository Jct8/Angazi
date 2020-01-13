#pragma once

#include "Colors.h"

namespace Angazi::GraphicsGL { class CameraGL; } // forward declare

namespace Angazi::GraphicsGL::SimpleDraw {

	void StaticInitialize(uint32_t maxVertexCount = 10000);
	void StaticTerminate();

	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
	//void AddBox(..., const Color& color);
	//void AddSphere(..., const Color& color);

	void Render(const CameraGL& camera);

} // namespace Angazi::GraphicsGL::SimpleDraw 