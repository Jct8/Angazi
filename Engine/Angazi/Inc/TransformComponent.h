#pragma once

#include "Component.h"

namespace Angazi
{
	class TransformComponent : public Component
	{
	public:

		void SetPosition(const Math::Vector3& position) { mPosition = position; }
		void SetRotation(const Math::Vector3& rotation) { mRotation = rotation; }
		void SetScale(const Math::Vector3& scale) { mScale = scale; }

		Math::Vector3 GetPosition() { return mPosition; }
		Math::Vector3 GetRotation() { return mRotation; }
		Math::Vector3 GetScale() { return mScale; }
		Math::Matrix4 GetTransform() { return Math::Matrix4::Transform(mPosition, mRotation, mScale); }

		void TranslateX(float value) { mPosition.x += value; }
		void TranslateY(float value) { mPosition.y += value; }
		void TranslateZ(float value) { mPosition.x += value; }

		void RotateX(float degree) { mRotation.x += degree; }
		void RotateY(float degree) { mRotation.y += degree; }
		void RotateZ(float degree) { mRotation.x += degree; }

		void ScaleX(float s) { mScale.x += s; }
		void ScaleY(float s) { mScale.y += s; }
		void ScaleZ(float s) { mScale.x += s; }

	private:
		Math::Vector3 mPosition = Math::Vector3::Zero;
		Math::Vector3 mRotation = Math::Vector3::Zero;
		Math::Vector3 mScale = Math::Vector3::One;
	};
}