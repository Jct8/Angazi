#include "Precompiled.h"
#include "LightService.h"

#include "ImGui/Inc/imgui.h"

using namespace Angazi;

META_DERIVED_BEGIN(LightService, Service)
	META_NO_FIELD
META_CLASS_END

void LightService::Initialize()
{
	mActiveLight = AddLight("Default");
}

void LightService::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Directional Lights", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (auto& [name, light] : mLightMap)
		{
			ImGui::Text(name.c_str());
			bool active = false;
			if (&light == mActiveLight)
				active = true;
			ImGui::PushID(name.c_str());

			if (ImGui::Checkbox("Active", &active) && active)
				mActiveLight = &light;

			ImGui::Columns(2, "DirectionalLights");

			ImGui::Text("Ambient Light Color");
			ImGui::NextColumn();
			ImGui::ColorEdit4("##DirectionalAmbient", &light.ambient.r, ImGuiColorEditFlags_NoInputs);
			ImGui::NextColumn();

			ImGui::Text("Diffuse Light Color");
			ImGui::NextColumn();
			ImGui::ColorEdit4("##DirectionalDiffuse", &light.diffuse.r, ImGuiColorEditFlags_NoInputs);
			ImGui::NextColumn();

			ImGui::Text("Specular Light Color");
			ImGui::NextColumn();
			ImGui::ColorEdit4("##DirectionalSpecular", &light.specular.r, ImGuiColorEditFlags_NoInputs);
			ImGui::NextColumn();

			ImGui::Text("Light Direction - X");
			ImGui::NextColumn();
			ImGui::DragFloat("##DirectionalDirX", &light.direction.x, 0.01f, -1.0f, 1.0f);
			ImGui::NextColumn();

			ImGui::Text("Light Direction - Y");
			ImGui::NextColumn();
			ImGui::DragFloat("##DirectionalDirY", &light.direction.y, 0.01f, -1.0f, 1.0f);
			ImGui::NextColumn();
			
			ImGui::Text("Light Direction - Z");
			ImGui::NextColumn();
			ImGui::DragFloat("##DirectionalDirZ", &light.direction.z, 0.01f, -1.0f, 1.0f);
			ImGui::NextColumn();

			ImGui::Columns(1);
			ImGui::PopID();
			ImGui::Separator();
		}
	}
}

Graphics::DirectionalLight* Angazi::LightService::AddLight(const char* name)
{
	return &mLightMap[name];
}

Graphics::DirectionalLight* Angazi::LightService::FindLight(const char* name)
{
	auto iter = mLightMap.find(name);
	return (iter == mLightMap.end() ? nullptr : &(iter->second));
}

void LightService::SetActiveLight(const char* name)
{
	auto Light = FindLight(name);
	if (Light)
		mActiveLight = Light;
}

Graphics::DirectionalLight& Angazi::LightService::GetActiveLight()
{
	ASSERT(mActiveLight, "LightService -- No active Light!");
	return *mActiveLight;
}

const Graphics::DirectionalLight& Angazi::LightService::GetActiveLight() const
{
	ASSERT(mActiveLight, "LightService -- No active Light!");
	return *mActiveLight;
}

