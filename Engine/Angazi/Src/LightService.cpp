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
		for (auto& [name,light] : mLightMap)
		{
			ImGui::Text(name.c_str());
			ImGui::ColorEdit4("Ambient Light", &light.ambient.r);
			ImGui::ColorEdit4("Diffuse Light", &light.diffuse.r);
			ImGui::ColorEdit4("Specular Light", &light.specular.r);
			ImGui::DragFloat3("Light Direction", &light.direction.x,0.01f,-1.0f,1.0f);
			if (ImGui::Button("Set as active light"))
				mActiveLight = &light;
			ImGui::Separator();
		}
	}
}

Graphics::DirectionalLight * Angazi::LightService::AddLight(const char * name)
{
	return &mLightMap[name];
}

Graphics::DirectionalLight* Angazi::LightService::FindLight(const char * name)
{
	auto iter = mLightMap.find(name);
	return (iter == mLightMap.end() ? nullptr : &(iter->second));
}

void LightService::SetActiveLight(const char * name)
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

