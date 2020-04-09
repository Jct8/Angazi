#pragma once

namespace Angazi::GraphicsGL
{
	class UniformBuffer
	{
	public:
		UniformBuffer() = default;
		~UniformBuffer();

		UniformBuffer(const UniformBuffer&) = delete;
		UniformBuffer& operator=(const UniformBuffer&) = delete;

		void Initialize(int size);
		void Terminate();
		void Bind(uint32_t slot = 0) const;
		void Update(const void* data) const;
	private:
		unsigned int mUniformBuffer = 0;
		int mSize;
	};

	template <class T>
	class TypedUniformBuffer : public UniformBuffer
	{
	public:
		using DataType = T;

		void Initialize()
		{
			static_assert(sizeof(DataType) % 16 == 0, "[TypedUniformBuffer] data type must be 16 byte aligned.");
			UniformBuffer::Initialize(sizeof(DataType));
		}

		void Set(const DataType& data) const
		{
			UniformBuffer::Update(&data);
		}
	};
}