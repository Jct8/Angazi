#pragma once

namespace Angazi::Graphics
{
	class ConstantBuffer
	{
	public:
		void Initialize(int size);
		void Terminate();
		void Bind();
		void Set(void* data);
	private:
		ID3D11Buffer* mConstantBuffer = nullptr;
	};

}