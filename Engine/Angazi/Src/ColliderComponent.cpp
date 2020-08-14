#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Angazi::ColliderComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void ColliderComponent::DebugUI()
{
	auto aabb = GetAABB();
	SimpleDraw::AddAABB(aabb.center, aabb.extend, Graphics::Colors::LightGreen);
}

Math::AABB Angazi::ColliderComponent::GetAABB() const
{
	// This is incorrect if we have orientation as well
	auto translation = mTransformComponent->position;
	return {translation + center,extend};
}
