#pragma once

namespace Angazi::Network
{
	class MemoryStream;

	class StreamReader
	{
	public:
		StreamReader(MemoryStream& memoryStream);

		template<class T>
		bool Read(T& data)
		{
			static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Error. Primitive types only.");
			return Read(&data, sizeof(T));
		}
		bool Read(std::string &data);

	private:
		bool Read(void *data, uint32_t size);
		MemoryStream& mMemoryStream;
	};
}