#include "Precompiled.h"
#include "MetaArray.h"

using namespace Angazi::Core::Meta;

Angazi::Core::Meta::MetaArray::MetaArray(const MetaType * elementType)
	:MetaType(MetaType::Category::Array, "Array", sizeof(std::vector<int>)), mElementType(elementType)
{
}
