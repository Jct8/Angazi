#pragma once

namespace Angazi::Network
{
	class MemoryStream
	{
	public:
		MemoryStream() = default;
		MemoryStream(uint32_t capacity);
		MemoryStream(const uint8_t* data,uint32_t size);
		~MemoryStream();

	public:
		void Rewind();
		const uint8_t* GetData() const { return mBuffer; }
		uint8_t GetSize() { return mHead; }

	private:
		MemoryStream(const MemoryStream&) = delete;
		MemoryStream& operator=(const MemoryStream&&) = delete;

		friend class StreamWriter;
		friend class StreamReader;

		void ReallocBuffer(uint32_t newCapacity);

		uint8_t *mBuffer = nullptr;
		uint32_t mHead = 0;
		uint32_t mCapacity = 0;
	};
}