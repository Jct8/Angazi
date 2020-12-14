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

		void Initialize(bool depthEnable, bool depthWrite);
		void Terminate();

		void Set() const;
		void Clear() const;

	private:
		bool mDepthEnabled = true;
		bool mDepthWrite = false;
	};
}

#endif