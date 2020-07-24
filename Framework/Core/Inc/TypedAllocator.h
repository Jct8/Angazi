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

		template <class... Args>
		DataType* New(Args... args)
		{
			void* ptr = Allocate();
			if (ptr == nullptr)
				return nullptr;
			DataType* retPtr = new(ptr) DataType(std::forward<Args>(args)...);
			return retPtr;
		}

		void Delete(DataType* ptr)
		{
			if (ptr == nullptr)
				return;
			ptr->~DataType();
			Free(ptr);
		}
	};
}