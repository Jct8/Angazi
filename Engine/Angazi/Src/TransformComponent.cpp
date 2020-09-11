#include "Precompiled.h"
#include "TransformComponent.h"

using namespace Angazi;
using namespace Angazi::Graphics;

META_DERIVED_BEGIN(TransformComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(position,"Position")
		META_FIELD(rotation,"Rotation")
		META_FIELD(scale,"Scale")
	META_FIELD_END
META_CLASS_END

void Angazi::TransformComponent::DebugUI()
{
	auto transform = GetTransform();
	SimpleDraw::AddTransform(transform);
}

Math::Matrix4 Angazi::TransformComponent::GetTransform()
{
	return Math::Matrix4::Transform(position, rotation, scale); 
}
