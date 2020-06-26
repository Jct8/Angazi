#pragma once

#ifdef ENABLE_DIRECTX11

namespace Angazi::Graphics
{
	class DepthStencilState
	{
	public:
		DepthStencilState() = default;
		~DepthStencilState();

		DepthStencilState(const DepthStencilState&) = delete;
		DepthStencilState& operator=(const DepthStencilState&) = delete;

		void Initialize(bool depthEnable, bool depthWrite);
		void Terminate();

		void Set();
		void Clear();

	private:
		ID3D11DepthStencilState* mDepthStencilState = nullptr;
	};
}

#endif