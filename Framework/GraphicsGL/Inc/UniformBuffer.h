#pragma once

#ifdef ENABLE_OPENGL
namespace Angazi::Graphics
{
	class ConstantBuffer
	{
	public:
		ConstantBuffer() = default;
		~ConstantBuffer();

		ConstantBuffer(const ConstantBuffer&) = delete;
		ConstantBuffer& operator=(const ConstantBuffer&) = delete;

		void Initialize(int size);
		void Terminate();
		void BindVS(uint32_t slot = 0) const;
		void BindPS(uint32_t slot = 0) const;
		void UnbindVS(uint32_t slot = 0) const;
		void UnbindPS(uint32_t slot = 0) const;


		void Update(const void* data) const;
	private:
		uint32_t mConstantBuffer = 0;
		int mSize;
	};

	template <class T>
	class TypedConstantBuffer : public ConstantBuffer
	{
	public:
		using DataType = T;

		void Initialize()
		{
			static_assert(sizeof(DataType) % 16 == 0, "[TypedConstantBuffer] data type must be 16 byte aligned.");
			ConstantBuffer::Initialize(sizeof(DataType));
		}

		void Set(const DataType& data) const
		{
			ConstantBuffer::Update(&data);
		}
	};
}

#endif