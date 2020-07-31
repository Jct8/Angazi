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
		using HandleType= Handle <DataType>;

		HandlePool(size_t capacity)
		{
			// TODO
			// allocate capacity + 1 entries (we don't use alot 0)
			// add freeslots to mFreeSlots (excluding 0)

			ASSERT(HandleType::sPool == nullptr, "HandlePool -- Cannot have more than one pool of this type!");
			HandleType::sPool = this;
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

			// TODO
			// Get the next dree slot
			// Set entry instance pointer
			// Return a handle to this entry (set index and generation)
		}
		void Unregister(HandleType handle)
		{
			// TODO 
			// Skip if handle is invalid
			// Else find the entry and increment the generation( this invalidates all existing handles to this slot)
			// Recycle the slot (add the index back to mFreeSlots)
		}
		void Flush()
		{
			// Loop throught all entries and increment generaton (invalidates all existing handles)
			// Re-add all slot indices to mFreeSlots
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