#pragma once
#ifdef ENABLE_OPENGL

namespace Angazi::Graphics
{
	class RasterizerState
	{
	public:
		enum class CullMode { Front, Back, None };
		enum class FillMode { Solid, Wireframe };

		RasterizerState() = default;
		~RasterizerState();

		RasterizerState(const RasterizerState&) = delete;
		RasterizerState& operator=(const RasterizerState&) = delete;

		void Initialize(CullMode cullMode, FillMode fillMode);
		void Terminate();

		void Set();
		void Clear();

	private:
		//ID3D11RasterizerState* mRasterizerState = nullptr;
	};
}

#endif