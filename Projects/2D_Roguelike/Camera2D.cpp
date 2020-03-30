#include "Camera2D.h"

using namespace Angazi;

namespace
{
	std::unique_ptr<Camera2D> sInstance;
}

void Camera2D::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "Camera already initialized");
	sInstance = std::make_unique<Camera2D>();
}

void Camera2D::StaticTerminate()
{
	sInstance.reset();
}

Camera2D & Camera2D::Get()
{
	ASSERT(sInstance != nullptr, "No Camera created!");
	return *sInstance;
}

Math::Vector2 Camera2D::ConvertToScreenPosition(const Math::Vector2 & worldPos) const
{
	return worldPos - mOffset;
}

Math::Rect Camera2D::ConvertToScreenPosition(const Math::Rect & worldPos) const
{
	Math::Rect rect;
	rect.left = worldPos.left - mOffset.x;
	rect.top = worldPos.top - mOffset.y;
	rect.right = worldPos.right - mOffset.x;
	rect.bottom = worldPos.bottom - mOffset.y;
	return rect;
}

Math::LineSegment Camera2D::ConvertToScreenPosition(const Math::LineSegment & worldPos) const
{
	Math::LineSegment line;
	line.from.x = worldPos.from.x - mOffset.x;
	line.from.y = worldPos.from.y - mOffset.y;
	line.to.x = worldPos.to.x - mOffset.x;
	line.to.y = worldPos.to.y - mOffset.y;
	return line;
}

Math::Vector2 Camera2D::ConvertToWorldPosition(const Math::Vector2 & screenPos) const
{
	return screenPos + mOffset;
}

void Camera2D::SetViewBound(const Math::Rect & viewBound)
{
	mViewBound = viewBound;
	/*float screenWidth = static_cast<float>(X::GetScreenWidth());
	float screenHeight = static_cast<float>(X::GetScreenHeight());
	if (mViewBound.right - mViewBound.left < screenWidth)
		mViewBound.right = mViewBound.left + screenWidth;
	if (mViewBound.bottom - mViewBound.top < screenHeight)
		mViewBound.bottom = mViewBound.top + screenHeight;*/
}

void Camera2D::SetViewPosition(const Math::Vector2 & viewPos)
{
	auto width = Graphics::GraphicsSystem::Get()->GetBackBufferWidth();
	auto height = Graphics::GraphicsSystem::Get()->GetBackBufferHeight();

	float screenWidth = static_cast<float>(width);
	float screenHeight = static_cast<float>(height);
	Math::Vector2 halfScreen = { width *0.5f, height *0.5f };

	mOffset = viewPos - halfScreen;

	/*if (mOffset.x < mViewBound.left)
		mOffset.x = mViewBound.left;
	if (mOffset.y < mViewBound.top)
		mOffset.y = mViewBound.top;
	if (mOffset.x > mViewBound.right - screenWidth)
		mOffset.x = mViewBound.right - screenWidth;
	if (mOffset.y > mViewBound.bottom - screenHeight)
		mOffset.y = mViewBound.bottom - screenHeight;*/
}
