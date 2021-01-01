#include "Precompiled.h"
#include "MetaRegistration.h"

namespace Angazi::Core::Meta
{
	// Deserialize
	template<>
	void Deserialize<uint32_t>(void* instance, const rapidjson::Value& value)
	{
		*(uint32_t*)instance = value.GetUint();
	}
	template<>
	void Deserialize<int>(void* instance, const rapidjson::Value& value)
	{
		*(int*)instance = value.GetInt();
	}
	template<>
	void Deserialize<float>(void* instance, const rapidjson::Value& value)
	{
		*(float*)instance = value.GetFloat();
	}
	template<>
	void Deserialize<bool>(void* instance, const rapidjson::Value& value)
	{
		*(bool*)instance = value.GetBool();
	}
	template<>
	void Deserialize<std::string>(void* instance, const rapidjson::Value& value)
	{
		*(std::string*)(instance) = value.GetString();
	}
	template<>
	void Deserialize<std::filesystem::path>(void* instance, const rapidjson::Value& value)
	{
		*(std::filesystem::path*)(instance) = value.GetString();
	}

	// Serialize
	template<>
	void Serialize<uint32_t>(const void* instance, rapidjson::Value& value, rapidjson::Document& document)
	{
		value.SetUint(*(uint32_t*)instance);
	}
	template<>
	void Serialize<int>(const void* instance, rapidjson::Value& value, rapidjson::Document& document)
	{
		value.SetInt(*(int*)instance);
	}
	template<>
	void Serialize<float>(const void* instance, rapidjson::Value& value, rapidjson::Document& document)
	{
		value.SetFloat(*(float*)instance);
	}
	template<>
	void Serialize<bool>(const void* instance, rapidjson::Value& value, rapidjson::Document& document)
	{
		value.SetBool(*(bool*)instance);
	}
	template<>
	void Serialize<std::string>(const void* instance, rapidjson::Value& value, rapidjson::Document& document)
	{
		value.SetString((*(std::string*)(instance)).c_str(),document.GetAllocator());
	}
	template<>
	void Serialize<std::filesystem::path>(const void* instance, rapidjson::Value& value, rapidjson::Document& document)
	{
		value.SetString((*(std::filesystem::path*)(instance)).u8string().c_str(),document.GetAllocator());
	}
}

	template<> 
	const Angazi::Core::Meta::MetaType* Angazi::Core::Meta::GetMetaType<void>()
	{
		static const Angazi::Core::Meta::MetaType sMetaType(
			Angazi::Core::Meta::MetaType::Category::Primitive,"void", 0,
			nullptr,
			nullptr);
		return &sMetaType;
	}

// Primitive Type Declarations
META_TYPE_DEFINE(int, Integer)
META_TYPE_DEFINE(float, Number)
META_TYPE_DEFINE(bool, Boolean)
META_TYPE_DEFINE(std::string, String)
META_TYPE_DEFINE(std::filesystem::path, Path)

void Angazi::Core::StaticMetaRegister()
{
	// Add MetaClass registration here...
	// eg. META_REGISTER(Foo);
}
