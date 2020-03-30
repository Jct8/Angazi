#pragma once
#include<Angazi/Inc/Angazi.h>

class Camera2D
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static Camera2D& Get();
public:
	Angazi::Math::Vector2 ConvertToScreenPosition(const Angazi::Math::Vector2& worldPos) const;
	Angazi::Math::Rect ConvertToScreenPosition(const Angazi::Math::Rect& worldPos) const;
	Angazi::Math::LineSegment ConvertToScreenPosition(const Angazi::Math::LineSegment& worldPos) const;
	Angazi::Math::Vector2 ConvertToWorldPosition(const Angazi::Math::Vector2& screenPos) const;

	void SetViewBound(const Angazi::Math::Rect& rect);
	void SetViewPosition(const Angazi::Math::Vector2& viewPos);

	Angazi::Math::Vector2 GetOffSet() { return mOffset; }
private:
	Angazi::Math::Rect mViewBound{ -FLT_MAX, -FLT_MAX , FLT_MAX ,FLT_MAX };
	Angazi::Math::Vector2 mOffset;
};
