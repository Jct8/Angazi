#pragma once

#include "Common.h"

//Macro to add Runtime Type Information
#define RTTI(ClassName)\
	static std::string GetTypeName() { return #ClassName; }\
	static std::size_t GetTypeId() { return std::hash<std::string>{}(GetTypeName()); }\
	static void* CreateInstance() { return new ClassName(); }

namespace Angazi::Network
{
	class ObjectFactory
	{
	public:
		template <class T>
		void Register()
		{
			mCreators[T::GetTypeId()] = T::CreateInstance;
		}
		void* CreateInstance(std::size_t typeId)
		{
			ASSERT(mCreators.find(typeId) != mCreators.end(), "ObjectFactory- Type id %d not registered.", static_cast<int>(typeId));
			return mCreators[typeId]();
		}
		template <class T>
		T* CreateInstance()
		{
			return reinterpret_cast<T*>(CreateInstance(T::GetTypeId()));
		}

	private:
		std::unordered_map<std::size_t, std::function<void*()>> mCreators;

	};
}