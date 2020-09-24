#pragma once

#include "Component.h"

namespace Angazi
{
	class TransformComponent : public Component
	{
	public:
		META_CLASS_DECLARE;

		void Initialize() override;
		void DebugUI() override;
		void ShowInspectorProperties() override;

		Math::Matrix4 GetTransform() const;

		void RotateX(float degree) { rotationDeg.x += degree; UpdateRotationQuaternion(); }
		void RotateY(float degree) { rotationDeg.y += degree; UpdateRotationQuaternion(); }
		void RotateZ(float degree) { rotationDeg.x += degree; UpdateRotationQuaternion(); }
		void SetRotation(const Math::Vector3& rotation) { rotationDeg = rotation; UpdateRotationQuaternion(); }
		void SetRotation(const Math::Quaternion& rotationQuat) { rotation = rotationQuat; UpdateRotationVec(); }

		Math::Vector3 GetRotation() { return rotationDeg; }
		Math::Quaternion GetRotationQuaternion() { return rotation; }

		Math::Vector3 position = Math::Vector3::Zero;
		Math::Vector3 scale = Math::Vector3::One;

	private:
		void UpdateRotationQuaternion();
		void UpdateRotationVec();

		Math::Quaternion rotation = Math::Quaternion::Identity;
		Math::Vector3 rotationDeg = Math::Vector3::Zero;
	};
}