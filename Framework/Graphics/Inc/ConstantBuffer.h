#pragma once

namespace Angazi::Graphics
{
	/*struct Vertex
	{
		Angazi::Math::Vector3 position;
		Color color;
	};

	struct Mesh
	{
		std::vector<Vertex> mVertices;
		std::vector<uint32_t> mIndices;
	};*/
	

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