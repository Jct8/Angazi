#include "Precompiled.h"
#include "MetaRegistration.h"

#include "Component.h"
#include "GameObject.h"

// Components
#include "ColliderComponent.h"
#include "TransformComponent.h"

using namespace Angazi;

void Angazi::StaticMetaRegister()
{
	// Add MetaClass registration here...
	// eg. META_REGISTER(Foo);

	META_REGISTER(Component);
	META_REGISTER(GameObject);

	META_REGISTER(ColliderComponent);
	META_REGISTER(TransformComponent);
}
