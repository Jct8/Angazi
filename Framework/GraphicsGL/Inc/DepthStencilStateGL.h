#pragma once

#ifdef ENABLE_OPENGL

namespace Angazi::Graphics
{
	class DepthStencilState
	{
	public:
		DepthStencilState() = default;
		~DepthStencilState();

		DepthStencilState(const DepthStencilState&) = delete;
		DepthStencilState& operator=(const DepthStencilState&) = delete;

		void Initialize(bool depthEnabled);
		void Terminate();

		void Set();
		void Clear();

	private:
		//ID3D11DepthStencilState* mDepthStencilState = nullptr;
	};
}

#endif