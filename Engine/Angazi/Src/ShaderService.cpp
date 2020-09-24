#include "Precompiled.h"
#include "ShaderService.h"

#include "GameWorld.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;

META_DERIVED_BEGIN(ShaderService, Service)
	META_NO_FIELD
META_CLASS_END;

void ShaderService::Terminate()
{
	for (auto& [name, effect] : mShaderMap)
		effect->Terminate();
}

void ShaderService::ShowInspectorProperties()
{
	if (ImGui::CollapsingHeader("Available Shaders", ImGuiTreeNodeFlags_DefaultOpen))
	{
	}
}

Graphics::Effect* ShaderService::GetShader(const char* name)
{
	auto iter = mShaderMap.find(name);
	return iter == mShaderMap.end() ? nullptr : iter->second.get();
}
