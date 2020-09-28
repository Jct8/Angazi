#pragma once

namespace Angazi::Core::Meta
{
	class MetaClass;

	// Register meta class so we can look up later by name
	void Register(const MetaClass* metaClass);

	// Look up meta class by name
	const MetaClass* FindMetaClass(const char* className);

	const std::map<std::string, const Meta::MetaClass*> GetAllMetaClasses();
}

#define META_REGISTER(Class)\
	Angazi::Core::Meta::Register(Class::StaticGetMetaClass())