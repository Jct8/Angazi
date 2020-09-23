#include "Precompiled.h"
#include "MetaRegistration.h"

namespace Angazi::Core::Meta
{
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
