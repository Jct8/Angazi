#include "Precompiled.h"
#include "StreamWriter.h"

#include "MemoryStream.h"

using namespace Angazi::Network;

StreamWriter::StreamWriter(MemoryStream & memoryStream)
	:mMemoryStream(memoryStream)
{
}

void StreamWriter::Write(const std::string & data)
{
	int size = static_cast<int>(data.size());
	Write(size);
	Write(data.data(), size);
}

void StreamWriter::Write(const void * data, uint32_t size)
{
	if (mMemoryStream.mHead + size > mMemoryStream.mCapacity)
	{
		mMemoryStream.ReallocBuffer(mMemoryStream.mCapacity + size);
	}
	memcpy(mMemoryStream.mBuffer + mMemoryStream.mHead, data, size);
	mMemoryStream.mHead += size;
}
