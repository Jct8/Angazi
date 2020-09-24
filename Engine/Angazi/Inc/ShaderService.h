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
		EffectType* AddShader(std::filesystem::path shaderpath = "None")
		{
			static_assert(std::is_base_of_v<Graphics::Effect, EffectType>, "ShaderService -- 'EffectType' must be derived from type 'Effect'.");
			auto& newShader = mShaders.emplace_back(std::make_unique<EffectType>());
			if (shaderpath == "None")
				static_cast<EffectType*>(newShader.get())->Initialize();
			else
				static_cast<EffectType*>(newShader.get())->Initialize(shaderpath);
			return static_cast<EffectType*>(newShader.get());
		}

		template <class EffectType>
		const EffectType* GetShader() const
		{
			for (auto& shader: mShaders)
			{
				if (shader->GetMetaClass() == EffectType::StaticGetMetaClass())
					return static_cast<const EffectType*>(shader.get());
			}
			return nullptr;
		}

		template <class EffectType>
		EffectType* GetShader()
		{
			const ShaderService* constMe = static_cast<const ShaderService*>(this);
			return const_cast<EffectType*>(constMe->GetShader<EffectType>());
		}

		//Graphics::Effect* GetShader(const char* name);

	private:
		//using ShaderMap = std::map<std::string, std::unique_ptr<Graphics::Effect>>;
		std::vector<std::unique_ptr<Graphics::Effect>> mShaders;
	};
}