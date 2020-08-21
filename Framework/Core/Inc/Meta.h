#pragma once

#include "MetaType.h"
#include "MetaUtil.h"

#define META_TYPE_DECLARE(Type)\
	template<> const Angazi::Core::Meta::MetaType* Angazi::Core::Meta::GetMetaType<Type>();

#define META_TYPE_DEFINE(Type,Name)\
	template<> const Angazi::Core::Meta::MetaType* Angazi::Core::Meta::GetMetaType<Type>()\
	{\
		static const Angazi::Core::Meta::MetaType sMetaType(Angazi::Core::Meta::MetaType::Category::Primitive,#Name, sizeof(Type));\
		return &sMetaType;\
	}
