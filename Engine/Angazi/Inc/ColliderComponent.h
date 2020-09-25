#pragma once

#include "Component.h"

namespace Angazi
{
	class TransformComponent;

	class ColliderComponent : public Component
	{
	public:
		META_CLASS_DECLARE;

		void Initialize() override;
		void DebugUI() override;
		void ShowInspectorProperties() override;

		Math::AABB GetAABB() const;
		Math::OBB GetOBB() const;
		Math::AABB GetSphere() const;

		Math::Vector3 center = Math::Vector3::Zero;
		Math::Vector3 extend = Math::Vector3::One;

	private:
		const TransformComponent* mTransformComponent = nullptr;
	};
}