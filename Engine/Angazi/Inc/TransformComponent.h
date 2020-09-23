#pragma once

#include "Component.h"

namespace Angazi
{
	class TransformComponent : public Component
	{
	public:
		META_CLASS_DECLARE;

		void DebugUI() override;
		void ShowInspectorProperties() override;

		Math::Matrix4 GetTransform() const;

		Math::Vector3 position = Math::Vector3::Zero;
		Math::Quaternion rotation = Math::Quaternion::Identity;
		Math::Vector3 scale = Math::Vector3::One;
		//Math::Vector3 rotation = Math::Vector3::Zero;

		//void SetPosition(const Math::Vector3& position) { mPosition = position; }
		//void SetRotation(const Math::Vector3& rotation) { mRotation = rotation; }
		//void SetScale(const Math::Vector3& scale) { mScale = scale; }

		//Math::Vector3 GetPosition() { return mPosition; }
		//Math::Vector3 GetRotation() { return mRotation; }
		//Math::Vector3 GetScale() { return mScale; }

		//void TranslateX(float value) { mPosition.x += value; }
		//void TranslateY(float value) { mPosition.y += value; }
		//void TranslateZ(float value) { mPosition.x += value; }

		//void RotateX(float degree) { mRotation.x += degree; }
		//void RotateY(float degree) { mRotation.y += degree; }
		//void RotateZ(float degree) { mRotation.x += degree; }

		//void ScaleX(float s) { mScale.x += s; }
		//void ScaleY(float s) { mScale.y += s; }
		//void ScaleZ(float s) { mScale.x += s; }
	};
}