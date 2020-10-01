#include "Precompiled.h"
#include "CameraService.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;

META_DERIVED_BEGIN(CameraService, Service)
META_NO_FIELD
META_CLASS_END

void CameraService::Initialize()
{
	mActiveCamera = AddCamera("Default");
}

void CameraService::ShowInspectorProperties()
{
	for (auto& [name, camera] : mCameraMap)
	{
		bool active = false;
		ImGui::PushID(name.c_str());
		std::string headerName = name;
		if (&camera == mActiveCamera)
		{
			active = true;
			headerName.append(" (Active)");
		}

		if (ImGui::CollapsingHeader(headerName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Columns(2, "CameraService");

			ImGui::Text("Active");
			ImGui::NextColumn();
			if (ImGui::Checkbox("##Active",&active) && active)
				mActiveCamera = &camera;
			ImGui::NextColumn();

			ImGui::Text("FOV");
			ImGui::NextColumn();
			float fov = camera.GetFOV();
			if (ImGui::DragFloat("##fov", &fov, 1.0f, 10.0f, 170.0f, "%.1f"))
				camera.SetFov(fov * Math::Constants::DegToRad);
			ImGui::NextColumn();

			ImGui::Text("Near Plane");
			ImGui::NextColumn();
			float nearPlane = camera.GetNearPlane();
			if (ImGui::DragFloat("##near", &nearPlane, 0.1f, 0.1f, 10000.0f, "%.2f"))
				camera.SetNearPlane(nearPlane);
			ImGui::NextColumn();

			ImGui::Text("Far Plane");
			ImGui::NextColumn();
			float farPlane = camera.GetFarPlane();
			if (ImGui::DragFloat("##far", &farPlane, 1.0f, 0.1f, 10000.0f, "%.2f"))
				camera.SetFarPlane(farPlane);
			ImGui::NextColumn();

			ImGui::Columns(1);
		}
		ImGui::PopID();
	}
}

Graphics::Camera* Angazi::CameraService::AddCamera(const char* name)
{
	return &mCameraMap[name];
}

Graphics::Camera* Angazi::CameraService::FindCamera(const char* name)
{
	auto iter = mCameraMap.find(name);
	return (iter == mCameraMap.end() ? nullptr : &(iter->second));
}

void Angazi::CameraService::SetActiveCamera(const char* name)
{
	auto camera = FindCamera(name);
	if (camera)
		mActiveCamera = camera;
}

Graphics::Camera& Angazi::CameraService::GetActiveCamera()
{
	ASSERT(mActiveCamera, "CameraService -- No active camera!");
	return *mActiveCamera;
}

const Graphics::Camera& Angazi::CameraService::GetActiveCamera() const
{
	ASSERT(mActiveCamera, "CameraService -- No active camera!");
	return *mActiveCamera;
}
