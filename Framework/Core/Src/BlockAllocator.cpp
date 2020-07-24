#include "Precompiled.h"
#include "BlockAllocator.h"

#include "Debug.h"

using namespace Angazi;
using namespace Angazi::Core;

BlockAllocator::BlockAllocator(size_t blockSize, size_t capacity)
	: mData(nullptr)
	, mBlockSize(blockSize)
	, mCapacity(capacity)
{
	ASSERT(capacity > 0, "BlockAllocator -- Invalid capacity.");
	// TODO
}

BlockAllocator::~BlockAllocator()
{
	// TODO
}

void* BlockAllocator::Allocate()
{
	return nullptr;
}

void BlockAllocator::Free(void* ptr)
{
	// TODO
}

