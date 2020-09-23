#include "Precompiled.h"
#include "MetaRegistration.h"

#include "Material.h"
using namespace Angazi::Graphics;

// Primitive Type Declarations
//

void Angazi::Graphics::StaticMetaRegister()
{
	// Add MetaClass registration here...
	// eg. META_REGISTER(Foo);
	
	META_REGISTER(Material);
}
