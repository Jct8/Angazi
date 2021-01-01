#include "Precompiled.h"
#include "MetaFunction.h"

#include "DebugUtil.h"

using namespace Angazi::Core::Meta;

MetaFunction::MetaFunction(const char* name, const MetaType* returnType, std::vector<const MetaType*> paramTypes)
	:mName(name), mReturnType(returnType), mParameterTypes(std::move(paramTypes))
{

}