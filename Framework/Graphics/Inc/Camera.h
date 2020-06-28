#pragma once

namespace Angazi::Graphics
{
	// This camera assumes YAxis as up direction. You cannot
	// look straight up nor straight down.
	class Camera
	{
	public:
		void SetPosition(const Math::Vector3 &position);
		void SetDirection(const Math::Vector3 &direction);
		void SetLookAt(const Math::Vector3 &target);

		// 3 degrees of freedom for translations
		void Strafe(float distance); //x  
		void Rise(float distance); // y
		void Walk(float distance); //z

		// 2 degrees of freedom for rotations
		void Pitch(float radian); 
		void Yaw(float radian);

		void SetFov(float fov);
		void SetAspectRatio(float ratio);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		// 1 degree of freedom for field of view
		void Zoom(float amount);

		const Math::Vector3 &GetPosition() const { return mPosition; }
		const Math::Vector3 &GetDirection() const { return mDirection; }
		float GetFarPlane() const { return mFarPlane; }

		Math::Matrix4 GetViewMatrix() const;
		Math::Matrix4 GetPerspectiveMatrix() const;

		Math::Matrix4 GetOrthographicMatrix(float width, float height) const;

	private:
		Math::Vector3 mPosition = Math::Vector3::Zero;
		Math::Vector3 mDirection = Math::Vector3::ZAxis;

		float mFov = 60.0f * Math::Constants::DegToRad;
		float mAspectRatio = 0.0f;
		float mNearPlane = 0.1f;
		float mFarPlane = 10000.0f;
};

} //namespace Angazi::Graphics