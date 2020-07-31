#include "Precompiled.h"
#include "BlockAllocator.h"

#include "DebugUtil.h"

using namespace Angazi;
using namespace Angazi::Core;

BlockAllocator::BlockAllocator(size_t blockSize, size_t capacity)
	: mData(nullptr), mBlockSize(blockSize), mCapacity(capacity)
{
	ASSERT(capacity > 0, "BlockAllocator -- Invalid capacity.");

	mData = malloc(blockSize* capacity);
	mFreeSlots.resize(capacity);
	for (size_t i = 0; i < capacity; ++i)
		mFreeSlots[i] = i;
}

BlockAllocator::~BlockAllocator()
{
	free(mData);
	mData = nullptr;
}

void* BlockAllocator::Allocate()
{
	if (mFreeSlots.empty())
		return nullptr;

	size_t freeSlot = mFreeSlots.back();
	mFreeSlots.pop_back();
	return (uint8_t*) mData + freeSlot * mBlockSize;
}

void BlockAllocator::Free(void* ptr)
{
	size_t difference = (uint8_t*) ptr - (uint8_t*)mData;
	mFreeSlots.push_back(difference / mBlockSize);
}