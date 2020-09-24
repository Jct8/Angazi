#pragma once

#include "Service.h"

namespace Angazi
{
	class ShaderService : public Service
	{
	public:
		META_CLASS_DECLARE;

		void Terminate() override;

		void ShowInspectorProperties() override;

		template<class EffectType>
		EffectType* AddShader(const char* name)
		{
			static_assert(std::is_base_of_v<Graphics::Effect, EffectType>, "ShaderService -- 'EffectType' must be derived from type 'Effect'.");

			auto [newShader,success] = mShaderMap.emplace(name,std::make_unique<EffectType>());
			return static_cast<EffectType*>(newShader->second.get());
		}

		Graphics::Effect* GetShader(const char* name);

	private:
		using ShaderMap = std::map<std::string, std::unique_ptr<Graphics::Effect>>;
		ShaderMap mShaderMap;
	};
}