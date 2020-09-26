#include "Precompiled.h"
#include "MetaField.h"

using namespace Angazi::Core::Meta;

MetaField::MetaField(const MetaType * type, const char * name, size_t offset)
	:mType(type), mName(name), mOffset(offset)
{
}

void * MetaField::GetFieldInstance(void * classInstance) const
{
	return static_cast<uint8_t*>(classInstance) + mOffset;
}

const void* MetaField::GetFieldInstance(const void* classInstance) const
{
	return static_cast<const uint8_t*>(classInstance) + mOffset;
}
