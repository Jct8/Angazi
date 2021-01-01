#include "Precompiled.h"
#include "ShaderService.h"

#include "GameWorld.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;

META_DERIVED_BEGIN(ShaderService, Service)
	META_NO_FIELD
	META_NO_METHOD
META_CLASS_END;

void ShaderService::Terminate()
{
	for (auto& shader : mShaders)
		shader->Terminate();
}

void ShaderService::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Shaders Loaded", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (auto& shader : mShaders)
		{
			ImGui::Text(shader->GetMetaClass()->GetName());
		}
	}
}
