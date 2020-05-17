#include "Precompiled.h"
#include "Camera.h"
#include "Graphics.h"

using namespace Angazi;
using namespace Angazi::Graphics;


void Camera::SetPosition(const Math::Vector3 & position)
{
	mPosition = position;
}

void Camera::SetDirection(const Math::Vector3 & direction)
{
	mDirection = Math::Normalize(direction);
}

void Camera::SetLookAt(const Math::Vector3 & target)
{
	mDirection = target - mPosition;
}

void Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void Camera::Strafe(float distance)
{
	const Math::Vector3 right = Math::Normalize(Math::Cross(Math::Vector3::YAxis, mDirection));
	mPosition += right * distance;
}

void Camera::Rise(float distance)
{
	mPosition += Math::Vector3::YAxis * distance;
}

void Camera::Yaw(float radian)
{
	const Math::Matrix4 matRot = Math::Matrix4::RotationY(radian);
	mDirection = Math::TransformNormal(mDirection, matRot);
}

void Camera::Pitch(float radian)
{
	const Math::Vector3 right = Math::Normalize(Math::Cross(Math::Vector3::YAxis, mDirection));
	const Math::Matrix4 matRot = Math::Matrix4::RotationAxis(right, radian);
	const Math::Vector3 newLook = Math::TransformNormal(mDirection, matRot);

	// Check to prevent our new look direction from being colinear with the Y-Axis
	const float dot = Math::Dot(newLook, Math::Vector3::YAxis);
	if (Math::Abs(dot) < 0.995f)
		mDirection = newLook;

}

void Camera::SetFov(float fov)
{
	constexpr float kMinFov = 10.0f * Math::Constants::DegToRad;
	constexpr float kMaxFov = 170.0f * Math::Constants::DegToRad;
	mFov = Math::Clamp(fov, kMinFov, kMaxFov);
}

void Camera::SetAspectRatio(float ratio)
{
	mAspectRatio = ratio;
}

void Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

void Camera::Zoom(float amount)
{
	constexpr float minZoom = 170.0f * Math::Constants::DegToRad;
	constexpr float maxZoom = 10.0f * Math::Constants::DegToRad;
	mFov = Math::Clamp(mFov - amount, maxZoom, minZoom);
}

Math::Matrix4 Camera::GetViewMatrix() const
{
	const Math::Vector3 l = mDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));

	const float dx = -Math::Dot(r, mPosition);
	const float dy = -Math::Dot(u, mPosition);
	const float dz = -Math::Dot(l, mPosition);

	return
	{
		r.x , u.x, l.x, 0.0f,
		r.y , u.y, l.y, 0.0f,
		r.z , u.z, l.z, 0.0f,
		dx  , dy , dz , 1.0f
	};

}

Math::Matrix4 Camera::GetPerspectiveMatrix() const
{
	float aspectRatio = mAspectRatio;
	if (aspectRatio == 0.0f)
	{
		const auto width = GraphicsSystem::Get()->GetBackBufferWidth();
		const auto height = GraphicsSystem::Get()->GetBackBufferHeight();
		aspectRatio = static_cast<float>(width) / height;
	}

	const float h = 1.0f / tan(mFov*0.5f);
	const float w = h / aspectRatio;
	const float zf = mFarPlane;
	const float zn = mNearPlane;
	const float d = zf / (zf - zn);

	return
	{
		w,		0.0f, 0.0f,		0.0f,
		0.0f,	h,    0.0f,		0.0f,
		0.0f,	0.0f, d ,		1.0f,
		0.0f,	0.0f, -zn * d,	0.0f
	};

}

Math::Matrix4 Camera::GetOrthographicMatrix(float width, float height) const
{
	const float w = width;
	const float h = height;
	const float zf = mFarPlane;
	const float zn = mNearPlane;
	return Math::Matrix4
	{
		2.0f / w , 0.0f    , 0.0f          , 0.0f,
		0.0f     , 2.0f / h, 0.0f          , 0.0f,
		0.0f     , 0.0f    , 1 / (zf - zn) , 0.0f,
		0.0f     , 0.0f    , zn / (zn - zf), 1.0f
	};
}
