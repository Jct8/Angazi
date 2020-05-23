#pragma once

#include "ConstantBuffer.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderTarget.h"
#include "Camera.h"

namespace Angazi::Graphics
{
	class Effect
	{
	public:
		enum EffectType
		{
			StandardType,
			ShadowType,
			NoEffect
		};
	public:
		Effect() = delete;
		Effect(EffectType type) : mType(type) {};
		virtual ~Effect() = default;
		Effect(const Effect&) = delete;
		Effect& operator=(const Effect&) = delete;

		virtual void Initialize(const std::filesystem::path& fileName) = 0;
		virtual void Terminate() = 0;

		virtual void Begin() = 0;
		virtual void End() = 0;
		EffectType GetEffectType() const { return mType; };

	protected:
		EffectType mType = NoEffect;
	};
}