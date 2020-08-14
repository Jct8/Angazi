#pragma once

#include "Component.h"

namespace Angazi
{
	class TransformComponent;

	class ColliderComponent : public Component
	{
	public:
		void Initialize() override;
		void DebugUI() override;

		Math::AABB GetAABB() const;
		Math::OBB GetOBB() const;
		Math::AABB GetSphere() const;

		Math::Vector3 center = Math::Vector3::Zero;
		Math::Vector3 extend = Math::Vector3::One;

		//void SetAABB(const Math::AABB& aabb) { mAABB = aabb; }
		//void SetCenter(const Math::Vector3& center) { mAABB.center = center; }
		//void SetScale(const Math::Vector3& scale) { mAABB.extend = scale; }

		//Math::AABB GetAABB() { return mAABB; }
		//Math::Vector3 GetCenter() { return mAABB.center; }
		//Math::Vector3 GetScale() { return mAABB.extend; }

		//void TranslateX(float value) { mAABB.center.x += value; }
		//void TranslateY(float value) { mAABB.center.y += value; }
		//void TranslateZ(float value) { mAABB.center.x += value; }

		//void ScaleX(float s) { mAABB.extend.x += s; }
		//void ScaleY(float s) { mAABB.extend.y += s; }
		//void ScaleZ(float s) { mAABB.extend.x += s; }

	private:
		const TransformComponent* mTransformComponent = nullptr;
	};
}