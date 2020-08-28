#include "Precompiled.h"
#include "StreamReader.h"

#include "MemoryStream.h"

using namespace Angazi::Network;

StreamReader::StreamReader(MemoryStream & memoryStream)
	:mMemoryStream(memoryStream)
{
	memoryStream.Rewind();
}

bool StreamReader::Read(std::string & data)
{
	int size = 0;
	Read(size);

	std::unique_ptr<char[]> inString(new char[size]);
	bool ret = Read(inString.get(), size);
	data.assign(inString.get(), size);

	return ret;
}

bool StreamReader::Read(void *data, uint32_t size)
{
	if ( mMemoryStream.mHead + size> mMemoryStream.mCapacity)
	{
		LOG("Capacity out of range error.");
		return false;
	}
	memcpy(data, mMemoryStream.mBuffer + mMemoryStream.mHead, size);
	mMemoryStream.mHead += size;
	return true;
}

