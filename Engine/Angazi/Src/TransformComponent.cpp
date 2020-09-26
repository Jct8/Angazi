#include "Precompiled.h"
#include "TransformComponent.h"

#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Math;
using namespace Angazi::Graphics;

META_DERIVED_BEGIN(TransformComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(position, "Position")
		META_FIELD(rotation, "Rotation")
		META_FIELD(scale, "Scale")
	META_FIELD_END
META_CLASS_END;

void TransformComponent::Initialize()
{
	rotation = Normalize(rotation);
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
		ImGui::Columns(2, "Transform");

		ImGui::Text("Position");
		ImGui::NextColumn();
		ImGui::DragFloat3("##TransformPosition", &position.x);
		ImGui::NextColumn();

		ImGui::Text("Rotation");
		ImGui::NextColumn();
		if (ImGui::DragFloat3("##TransformRotation", &rotationDeg.x))
			UpdateRotationQuaternion();
		ImGui::NextColumn();

		ImGui::Text("Scale");
		ImGui::NextColumn();
		ImGui::DragFloat3("##TransformScale", &scale.x);
		ImGui::NextColumn();
		ImGui::Columns(1);
	}
}

Math::Matrix4 TransformComponent::GetTransform() const
{
	return Math::Matrix4::Transform(position, Math::Normalize(rotation), scale);
}

void TransformComponent::UpdateRotationQuaternion()
{
	rotation = Quaternion::RotationAxis(Vector3::XAxis, rotationDeg.x * Constants::DegToRad) *
		Quaternion::RotationAxis(Vector3::YAxis, rotationDeg.y * Constants::DegToRad) *
		Quaternion::RotationAxis(Vector3::ZAxis, rotationDeg.z * Constants::DegToRad);
	rotation = Normalize(rotation);
}

void TransformComponent::UpdateRotationVec()
{
	rotationDeg = Vector3::ToEulerAngleXYZ(rotation) * Constants::RadToDeg;
}

