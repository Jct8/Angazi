#pragma once

#include "BlockAllocator.h"

namespace Angazi::Core
{
	template <class DataType>
	class TypedAllocator : private BlockAllocator
	{
	public:
		TypedAllocator(size_t capacity)
			:BlockAllocator(sizeof(DataType),capacity)
		{

		}

		DataType* New()
		{
			// TODO
			// Get a new block from BlockAllocator
			// Use placement new on the returned block
			return nullptr;
		}

		// Part 2
		//template <class... Args>
		//DataType* New()
		//{
		//	// TODO
		//	// Modify New() so it is using variadic template arguments and perfect forwarding
		//}

		void Delete(DataType* ptr)
		{
			// TODO
			// Call destructor on ptr
			// Return ptr to BlockAllocator
			// if ptr is nullptr do nothing
		}
	};
}