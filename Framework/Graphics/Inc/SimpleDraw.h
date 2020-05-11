#pragma once

#include "Colors.h"

namespace Angazi::Graphics { class Camera; } // forward declare

namespace Angazi::Graphics::SimpleDraw {

	void StaticInitialize(uint32_t maxVertexCount = 10000);
	void StaticTerminate();

	//3D Simple Draw
	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
	void AddAABB(const Math::Vector3& center, const Math::Vector3& extend, const Color & color, bool fill = false);
	void AddCone(float height, float radius, const Color& color, bool fill = false);
	void AddCone(const Math::Vector3& base, const Math::Vector3& direction, float height, float radius, const Color& color, bool fill = false);
	void AddCylinder(const Math::Vector3& base, const Math::Vector3& direction, float height, float radius, const Color& color, bool fill = false);
	void AddSphere(const Math::Vector3& center, float radius, const Color& color, bool fill = false, int slices = 8, int rings = 16);
	void AddSphere(const Math::Sphere& sphere,  const Color& color, bool fill = false, int slices = 8, int rings = 16);
	void AddSphere(float x, float y, float z, float radius, const Color& color, bool fill = false, int slices = 8, int rings = 16);

	void AddTransform(const Math::Matrix4& transform);
	void AddBone(const Math::Matrix4& transform);

	void AddGroundPlane(int size, bool fill = true ,const Color& color = Colors::DarkGray);

	//2D Simple Draw
	void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color);
	void AddScreenLine(float x0, float y0, float x1, float y1, const Color& color);
	void AddScreenCircle(const Math::Circle& circle, const Color& color);
	void AddScreenCircle(const Math::Vector2& center, float radius, const Color& color);
	void AddScreenCircle(float centerX, float centerY, float radius, const Color& color);
	void AddScreenRect(const Math::Rect& rect, const Color& color);
	void AddScreenRect(const Math::Vector2& min, const Math::Vector2& max, const Color& color);
	void AddScreenRect(float left, float top, float right, float bottom, const Color& color);

	void Render(const Camera& camera, const Math::Matrix4& matWorld = Math::Matrix4::Identity);

} // namespace Angazi::Graphics::SimpleDraw 