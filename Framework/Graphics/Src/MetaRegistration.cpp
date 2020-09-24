#include "Precompiled.h"
#include "MetaRegistration.h"

#include "Effect.h"
#include "Material.h"
#include "StandardEffect.h"
#include "PbrEffect.h"


using namespace Angazi::Graphics;

// Primitive Type Declarations
//

void Angazi::Graphics::StaticMetaRegister()
{
	// Add MetaClass registration here...
	// eg. META_REGISTER(Foo);
	
	META_REGISTER(Effect);
	META_REGISTER(Material);

	META_REGISTER(StandardEffect);
	META_REGISTER(PbrEffect);
}
