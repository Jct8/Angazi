#pragma once

#include "Common.h"
#include "DebugUtil.h"

namespace Angazi::Core
{
	template<class DataType>
	class Handle;

	template <class DataType>
	class HandlePool
	{
	public:
		using HandleType = Handle<DataType>;

		HandlePool(size_t capacity)
		{

			ASSERT(HandleType::sPool == nullptr, "HandlePool -- Cannot have more than one pool of this type!");
			HandleType::sPool = this;

			mEntries.resize(capacity + 1);					// allocate capacity + 1 entries (we don't use slot 0)
			mFreeSlots.resize(capacity);
			for (size_t i = 0; i < mFreeSlots.size(); i++)	// add freeslots to mFreeSlots (excluding 0)
				mFreeSlots[i] = i + 1;
		}
		~HandlePool()
		{
			ASSERT(HandleType::sPool == this, "HandlePool -- Something is wrong...");
			HandleType::sPool = nullptr;
		}

		HandleType Register(DataType* instance)
		{
			ASSERT(instance != nullptr, "HandlePool -- Invalid instance.");
			ASSERT(!mFreeSlots.empty(), "HandlePool -- No more free slots available.");

			if (mFreeSlots.empty())
				return HandleType();
			size_t freeSlot = mFreeSlots.back();
			mFreeSlots.pop_back();
			mEntries[freeSlot].instance = instance;

			HandleType handleType;
			handleType.mGeneration = mEntries[freeSlot].generation;
			handleType.mIndex = freeSlot;
			return handleType;

		}
		void Unregister(HandleType handle)
		{
			if (!IsValid(handle))
				return;

			mEntries[handle.mIndex].instance = nullptr;
			mEntries[handle.mIndex].generation++; // this invalidates all existing handles to this slot
			mFreeSlots.push_back(handle.mIndex);
			handle.Invalidate();
		}
		void Flush()
		{
			for (size_t i = 0; i < mEntries.size(); i++)
			{
				mEntries[i].instance = nullptr;
				mEntries[i].generation++; // invalidates all existing handles
			}
			mFreeSlots.clear();
			for (size_t i = 1; i < mEntries.size(); i++)
				mFreeSlots.push_back(i);
		}

		bool IsValid(HandleType handle) const
		{
			return handle != HandleType() && mEntries[handle.mIndex].generation == handle.mGeneration;
		}
		DataType* Get(HandleType handle)
		{
			return IsValid(handle) ? mEntries[handle.mIndex].instance : nullptr;
		}

	private:
		struct Entry
		{
			DataType* instance = nullptr;
			uint32_t generation = 0;
		};

		std::vector<Entry> mEntries;
		std::vector<size_t> mFreeSlots;
	};
}