#pragma once

#include "Camera.h"

namespace Angazi::Graphics
{
	class Effect
	{
	public:
		META_CLASS_DECLARE;

		enum class EffectType
		{
			StandardType,
			ShadowType,
			PostProcessingType,
			NoEffect
		};
	public:
		Effect() = default;
		Effect(EffectType type) : mType(type) {};
		virtual ~Effect() = default;

		Effect(const Effect&) = delete;
		Effect& operator=(const Effect&) = delete;

		virtual void Initialize(const std::filesystem::path& fileName = "NONE") {};
		virtual void Terminate() {}

		virtual void Begin() {};
		virtual void End() {};
		EffectType GetEffectType() const { return mType; };

	protected:
		EffectType mType = EffectType::NoEffect;
	};
}
