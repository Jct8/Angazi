#include "Precompiled.h"
#include "MetaRegistration.h"

#include "Effect.h"
#include "HdrEffect.h"
#include "Material.h"
#include "StandardEffect.h"
#include "ShadowEffect.h"
#include "PbrEffect.h"


using namespace Angazi::Graphics;

// Primitive Type Declarations

void Angazi::Graphics::StaticMetaRegister()
{
	// Add MetaClass registration here...
	// eg. META_REGISTER(Foo);
	
	META_REGISTER(Material);

	META_REGISTER(Effect);
	META_REGISTER(HdrEffect);
	META_REGISTER(PbrEffect);
	META_REGISTER(StandardEffect);
	META_REGISTER(ShadowEffect);
}
