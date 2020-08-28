#pragma once

namespace Angazi::Network
{
	class MemoryStream;

	class StreamWriter
	{
	public:
		StreamWriter(MemoryStream& memoryStream);

		template<class T>
		void Write(T& data)
		{
			static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Can only write primitive types.");
			Write(&data, sizeof(T));
		}
		void Write(const std::string &data);

	private:
		void Write(const void *data, uint32_t size);
		MemoryStream& mMemoryStream;
	};
}