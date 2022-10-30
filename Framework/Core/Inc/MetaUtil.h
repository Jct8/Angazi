#pragma once

namespace Angazi::Core::Meta
{
	class MetaType;

	template <class DataType>
	const MetaType* GetMetaType();

	// https://stackoverflow.com/questions/71867795/specialised-template-not-asserting-despite-compile-time-constant-static-assert
	// or compile with /permissive- option
	template<class...> constexpr bool alwaysFalse = false;

	template< class DataType>
	void Deserialize(void* instance, const rapidjson::Value& value)
	{
		static_assert(alwaysFalse<DataType>, "No specialization found for deserializing this type.");
	}
	template< class DataType>
	void Serialize(const void* instance, rapidjson::Value& value, rapidjson::Document& document)
	{
		static_assert(alwaysFalse<DataType>, "No specialization found for serializing this type.");
	}

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

	template <class ClassType, class ReturnType, class... Args>
	inline const MetaType* GetMethodReturnType(ReturnType (ClassType::*)(Args...)) noexcept
	{
		return GetMetaType<ReturnType>();
	}

	template <class ClassType, class ReturnType, class... Args>
	inline const MetaType* GetMethodReturnType(ReturnType(ClassType::*)(Args...) const) noexcept
	{
		return GetMetaType<ReturnType>();
	}

	template <class ClassType, class ReturnType, class... Args>
	inline auto GetMethodParameterTypes(ReturnType(ClassType::*)(Args...) ) noexcept
	{
		std::vector<const MetaType*> paramTypes;
		(paramTypes.push_back(GetMetaType<Args>()), ...);
		return paramTypes;
	}

	template <class ClassType, class ReturnType, class... Args>
	inline auto GetMethodParameterTypes(ReturnType(ClassType::*)(Args...) const) noexcept
	{
		std::vector<const MetaType*> paramTypes;
		(paramTypes.push_back(GetMetaType<Args>()), ...);
		return paramTypes;
	}
}