#include "Precompiled.h"
#include "TransformComponent.h"

#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Math;
using namespace Angazi::Graphics;

META_DERIVED_BEGIN(TransformComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(position,"Position")
		META_FIELD(rotation,"Rotation")
		META_FIELD(scale,"Scale")
	META_FIELD_END
META_CLASS_END;

void TransformComponent::Initialize()
{
	UpdateRotationVec();
}

void TransformComponent::DebugUI()
{
	auto transform = GetTransform();
	SimpleDraw::AddTransform(transform);
}

void TransformComponent::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Position", &position.x);
		if (ImGui::DragFloat3("Rotation", &rotationDeg.x))
			UpdateRotationQuaternion();
		ImGui::DragFloat3("Scale", &scale.x);
	}
}

Math::Matrix4 TransformComponent::GetTransform() const
{
	return Math::Matrix4::Transform(position, Math::Normalize(rotation), scale); 
}

void TransformComponent::UpdateRotationQuaternion()
{
	rotation  = Quaternion::RotationAxis(Vector3::XAxis, rotationDeg.x * Constants::DegToRad)*
				Quaternion::RotationAxis(Vector3::YAxis, rotationDeg.y * Constants::DegToRad)*
				Quaternion::RotationAxis(Vector3::ZAxis, rotationDeg.z * Constants::DegToRad);
}

void TransformComponent::UpdateRotationVec()
{
	rotationDeg = Vector3::ToEulerAngle(rotation);
}

