#pragma once
#ifdef ENABLE_OPENGL
namespace Angazi::Graphics
{
	class BlendState
	{
	public:
		static void ClearState();
	public:
		enum class Mode
		{
			Opaque,
			AlphaBlend,
			AlpahPremultiplied,
			Additive
		};

		BlendState() = default;
		~BlendState();

		BlendState(const BlendState&) = delete;
		BlendState& operator=(const BlendState&) = delete;

		void Initialize(Mode mode);
		void Terminate();

		void Bind() const;
	private:
		Mode mBlendState = Mode::Opaque;
	};
}

#endif