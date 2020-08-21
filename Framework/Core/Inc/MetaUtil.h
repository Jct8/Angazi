#pragma once

namespace Angazi::Core::Meta
{
	class MetaType;

	template <class DataType>
	const MetaType* GetMetaType();

	namespace Detail
	{
		template < class DataType>
		inline const MetaType* GetMetaType(DataType*)
		{
			static_assert(sizeof(DataType) == -1, "No MetaType found for this type.");
		}
	}

	template <class DataType>
	inline const MetaType* GetMetaType()
	{
		return Detail::GetMetaType(static_cast<DataType*>(nullptr));
	}
}