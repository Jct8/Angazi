#pragma once

#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaField.h"
#include "MetaPointer.h"
#include "MetaType.h"
#include "MetaUtil.h"

// -------------------------------------------------------------------------------------------------------------------

////////////////////////////// Meta Type Macros /////////////////////////////
#define META_TYPE_DECLARE(Type)\
	template<> const Angazi::Core::Meta::MetaType* Angazi::Core::Meta::GetMetaType<Type>();

#define META_TYPE_DEFINE(Type,Name)\
	template<> const Angazi::Core::Meta::MetaType* Angazi::Core::Meta::GetMetaType<Type>()\
	{\
		static const Angazi::Core::Meta::MetaType sMetaType(Angazi::Core::Meta::MetaType::Category::Primitive,#Name, sizeof(Type));\
		return &sMetaType;\
	}

// -------------------------------------------------------------------------------------------------------------------

////////////////////////////// Meta Class Macros /////////////////////////////
#define META_CLASS_DECLARE\
	static const Angazi::Core::Meta::MetaClass* StaticGetMetaClass();\
	virtual const Angazi::Core::Meta::MetaClass* GetMetaClass() const { return StaticGetMetaClass(); }

#define META_DERIVED_BEGIN(ClassType, ParentType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const Angazi::Core::Meta::MetaClass* parentMetaClass = ParentType::StaticGetMetaClass();

#define META_CLASS_BEGIN(ClassType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const Angazi::Core::Meta::MetaClass* parentMetaClass = nullptr;

#define META_CLASS_BEGIN_INTERNAL(ClassType)\
	template <> const Angazi::Core::Meta::MetaType* Angazi::Core::Meta::GetMetaType<ClassType>()\
	{\
		return ClassType::StaticGetMetaClass();\
	}\
	const Angazi::Core::Meta::MetaClass* ClassType::StaticGetMetaClass()\
	{\
		using Class = ClassType;\
		const char* className = #ClassType;



////////////////////////////// Meta Field Macros /////////////////////////////
#define META_FIELD_BEGIN\
		static const std::initializer_list<Angazi::Core::Meta::MetaField> fields {

#define META_FIELD(Var, Name)\
			{ Angazi::Core::Meta::GetFieldType(&Class::Var), Name, Angazi::Core::Meta::GetFieldOffset(&Class::Var) },

#define META_FIELD_END\
		};

#define META_NO_FIELD\
		static const std::initializer_list<Angazi::Core::Meta::MetaField> fields;

#define META_CLASS_END\
		static const Angazi::Core::Meta::MetaClass sMetaClass(\
			className, sizeof(Class),\
			parentMetaClass, fields);\
		return &sMetaClass;\
	}
