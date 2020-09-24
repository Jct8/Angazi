#include "Precompiled.h"
#include "EnvironmentService.h"

#include "GameWorld.h"
#include "CameraService.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;

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
	if (ImGui::CollapsingHeader("Available Environments", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (auto& [name, skybox] : mEnvironmentMap)
		{
			if (ImGui::Button(name.c_str()))
				mActiveSkybox = &skybox;
		}
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
