#include "Precompiled.h"
#include "EnvironmentService.h"

#include "GameWorld.h"
#include "CameraService.h"
#include "ImGui/Inc/imgui.h"
#include "ShaderService.h"
#include "LightService.h"

using namespace Angazi;
using namespace Angazi::Graphics;

META_DERIVED_BEGIN(EnvironmentService, Service)
	META_NO_FIELD
META_CLASS_END;

void EnvironmentService::Terminate()
{
	for (auto& [name, skybox] : mEnvironmentMap)
		skybox.Terminate();
}

void EnvironmentService::Render()
{
	auto& camera = GetWorld().GetService<CameraService>()->GetActiveCamera();
	if (mActiveSkybox)
		mActiveSkybox->Draw(camera);
}

void EnvironmentService::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Environment Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		const auto& shaderService = GetWorld().GetService<ShaderService>();
		const auto& hdrEffect = shaderService != nullptr ? shaderService->GetShader<HdrEffect>() : nullptr;

		ImGui::Columns(2, "EnvironmentService");

		ImGui::Text("Enable HDR");
		ImGui::NextColumn();
		if (ImGui::Checkbox("##EnableHDR", &useHDR))
		{
			if(hdrEffect)
				hdrEffect->EnableHDR(useHDR);
		}
		ImGui::NextColumn();

		ImGui::Text("Enable Gamma Correction");
		ImGui::NextColumn();
		if (ImGui::Checkbox("##EnableGammaCorrection", &useGammaCorrection))
		{
			if (hdrEffect)
				hdrEffect->EnableGammaCorrection(useGammaCorrection);
		}
		ImGui::NextColumn();

		ImGui::Text("Exposure");
		ImGui::NextColumn();
		if (ImGui::SliderFloat("##Exposure", &exposure, 0.0f, 10.0f))
		{
			if (hdrEffect)
				hdrEffect->SetExposure(exposure);
		}
		ImGui::NextColumn();

		ImGui::Columns(1);
	}
	for (auto& [name, camera] : mEnvironmentMap)
	{
		bool active = false;
		ImGui::PushID(name.c_str());
		std::string headerName = name;
		if (&camera == mActiveSkybox)
		{
			active = true;
			headerName.append(" (Active)");
		}

		if (ImGui::CollapsingHeader(headerName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Columns(2, "Skybox");

			ImGui::Text("Active");
			ImGui::NextColumn();
			if (ImGui::Checkbox("##ActiveSkybox", &active) && active)
				mActiveSkybox = &camera;
			ImGui::NextColumn();

			ImGui::Columns(1);
		}
		ImGui::PopID();
	}
}

Graphics::Skybox* EnvironmentService::AddEnvironment(const char* name)
{
	auto skybox = &mEnvironmentMap[name];
	if (!mActiveSkybox)
		mActiveSkybox = skybox;
	return skybox;
}

Graphics::Skybox* EnvironmentService::FindEnvironment(const char* name)
{
	auto iter = mEnvironmentMap.find(name);
	return (iter == mEnvironmentMap.end() ? nullptr : &(iter->second));
}

void EnvironmentService::SetActiveEnvironment(const char* name)
{
	auto skybox = FindEnvironment(name);
	if (skybox)
		mActiveSkybox = skybox;
}

Graphics::Skybox& EnvironmentService::GetActiveEnvironment()
{
	ASSERT(mActiveSkybox, "EnvironmentService -- No active environment!");
	return *mActiveSkybox;
}

const Graphics::Skybox& EnvironmentService::GetActiveEnvironment() const
{
	ASSERT(mActiveSkybox, "EnvironmentService -- No active environment!");
	return *mActiveSkybox;
}
