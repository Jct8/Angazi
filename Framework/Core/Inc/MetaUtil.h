#pragma once

namespace Angazi::Core::Meta
{
	class MetaType;

	template <class DataType>
	const MetaType* GetMetaType();

	namespace Detail
	{
		template < class DataType>
		inline const MetaType* GetMetaTypeImpl(DataType*)
		{
			static_assert(sizeof(DataType) == -1, "No MetaType found for this type.");
		}

		template < class DataType>
		inline const MetaType* GetMetaTypeImpl(DataType**)
		{
			static const MetaPointer sMetaAarray(GetMetaType<DataType>());
			return &sMetaAarray;
		}

		template < class DataType>
		inline const MetaType* GetMetaTypeImpl(std::vector<DataType>*)
		{
			static const MetaArray sMetaAarray(GetMetaType<DataType>());
			return &sMetaAarray;
		}
	}

	template <class DataType>
	inline const MetaType* GetMetaType()
	{
		return Detail::GetMetaTypeImpl(static_cast<DataType*>(nullptr));
	}

	template <class ClassType, class DataType>
	inline const MetaType* GetFieldType(DataType ClassType::*)
	{
		return GetMetaType<DataType>();
	}

	template <class ClassType, class DataType>
	inline size_t GetFieldOffset(DataType ClassType::* field)
	{
		return (size_t)(void*)&(((ClassType*)nullptr)->*field);
	}
}