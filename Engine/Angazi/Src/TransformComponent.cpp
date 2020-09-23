#include "Precompiled.h"
#include "TransformComponent.h"

#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;

META_DERIVED_BEGIN(TransformComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(position,"Position")
		META_FIELD(rotation,"Rotation")
		META_FIELD(scale,"Scale")
	META_FIELD_END
META_CLASS_END;

void Angazi::TransformComponent::DebugUI()
{
	auto transform = GetTransform();
	SimpleDraw::AddTransform(transform);
}

void TransformComponent::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Position", &position.x);
		ImGui::DragFloat4("Rotation", &rotation.x);
		ImGui::DragFloat3("Scale", &scale.x);
	}
}

Math::Matrix4 Angazi::TransformComponent::GetTransform() const
{
	return Math::Matrix4::Transform(position, Math::Normalize(rotation), scale); 
}
