#include "Precompiled.h"
#include "MetaRegistration.h"

#include "Component.h"
#include "GameObject.h"
#include "Service.h"

// Components
#include "ColliderComponent.h"
#include "TransformComponent.h"

// Services
#include "CameraService.h"
#include "EnvironmentService.h"

using namespace Angazi;

void Angazi::StaticMetaRegister()
{
	// Add MetaClass registration here...
	// eg. META_REGISTER(Foo);

	META_REGISTER(Component);
	META_REGISTER(GameObject);
	META_REGISTER(Service);

	META_REGISTER(ColliderComponent);
	META_REGISTER(TransformComponent);

	META_REGISTER(CameraService);
	META_REGISTER(EnvironmentService);
}
