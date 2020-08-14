#include "Precompiled.h"
#include "TransformComponent.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Angazi::TransformComponent::DebugUI()
{
	auto transform = GetTransform();
	SimpleDraw::AddTransform(transform);
}
