#include "Precompiled.h"
#include "ColliderComponent.h"

using namespace Angazi;

void ColliderComponent::Render()
{
	Graphics::SimpleDraw::AddAABB(mAABB.center, mAABB.extend, Graphics::Colors::AliceBlue);
}
