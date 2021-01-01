#include "Precompiled.h"
#include "MetaClass.h"

#include "DebugUtil.h"
#include "MetaField.h"
#include "MetaFunction.h"

using namespace Angazi::Core::Meta;

MetaClass::MetaClass(const char * name, size_t size, const MetaClass * parent, std::vector<MetaField> fields, CreateFunc create, std::vector<MetaFunction> methods)
	:MetaType(MetaType::Category::Class, name, size), mParent(parent), mFields(std::move(fields)), mCreate(std::move(create)),mMethods(std::move(methods))
{
}

const MetaClass * MetaClass::GetParent() const
{
	return mParent;
}

const MetaField * MetaClass::FindField(const char * name) const
{
	for (auto& field : mFields)
	{
		if (strcmp(field.GetName(), name) == 0)
			return &field;
	}

	// If the field is not found , try the parent
	if (mParent != nullptr)
		return mParent->FindField(name);

	return nullptr;
}

const MetaField * MetaClass::GetField(size_t index) const
{
	ASSERT(index < GetFieldCount(), "MetaClass -- Subscript out of range!");
	const size_t parentCount = GetParentFieldCount();
	if (index < parentCount)
		return mParent->GetField(index);
	return mFields.data() + (index - parentCount);
}

size_t MetaClass::GetFieldCount() const
{
	return mFields.size() + GetParentFieldCount();
}

size_t MetaClass::GetParentFieldCount() const
{
	return mParent ? mParent->GetFieldCount() : 0u;
}

size_t Angazi::Core::Meta::MetaClass::GetParentMethodCount() const
{
	return mParent ? mParent->GetMethodCount() : 0u;
}

void* MetaClass::Create() const
{
	ASSERT(mCreate, "MetaClass -- no create callable registered for %s.",GetName());
	return mCreate();
}

void MetaClass::Deserialize(void* classInstance, const rapidjson::Value& jsonValue) const
{
	for (auto& member : jsonValue.GetObjectW())
	{
		auto metaField = FindField(member.name.GetString());
		auto metaType = metaField->GetMetaType();
		metaType->Deserialize(metaField->GetFieldInstance(classInstance), member.value);
	}
}

void MetaClass::Serialize(const void* classInstance, rapidjson::Value& jsonValue, rapidjson::Document& document) const
{
	for (auto& metaField : mFields)
	{
		rapidjson::Value fieldName(rapidjson::kObjectType);
		rapidjson::Value fieldProperties(rapidjson::kObjectType);

		auto name = metaField.GetName();
		fieldName.SetString(metaField.GetName(), document.GetAllocator());
		
		auto metaType = metaField.GetMetaType();
		metaType->Serialize(metaField.GetFieldInstance(classInstance), fieldProperties, document);
		jsonValue.AddMember(fieldName, fieldProperties,document.GetAllocator());
	}
}

const MetaFunction* Angazi::Core::Meta::MetaClass::FindMethod(const char* name) const
{
	for (auto& method : mMethods)
	{
		if (strcmp(method.GetName(), name) == 0)
			return &method;
	}

	// If the field is not found , try the parent
	if (mParent != nullptr)
		return mParent->FindMethod(name);

	return nullptr;
}

const MetaFunction* Angazi::Core::Meta::MetaClass::GetMethod(size_t index) const
{
	ASSERT(index < GetMethodCount(), "MetaClass -- Subscript out of range!");
	const size_t parentCount = GetParentMethodCount();
	if (index < parentCount)
		return mParent->GetMethod(index);
	return mMethods.data() + (index - parentCount);
}

size_t Angazi::Core::Meta::MetaClass::GetMethodCount() const
{
	return mMethods.size() + GetParentMethodCount();
}
