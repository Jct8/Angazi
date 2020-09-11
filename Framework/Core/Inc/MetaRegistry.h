#pragma once

namespace Angazi::Core::Meta
{
	class MetaClass;

	// Register meta class so we can look up later by name
	void Register(const MetaClass* metaClass);

	// Look up meta class by name
	const MetaClass* FindMetaClass(const char* className);
}

#define META_REGISTER(Class)\
	Angazi::Core::Meta::Register(Class::StaticGetMetaClass())