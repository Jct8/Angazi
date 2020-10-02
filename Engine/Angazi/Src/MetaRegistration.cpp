#include "Precompiled.h"
#include "MetaRegistration.h"

#include "Component.h"
#include "GameObject.h"
#include "Service.h"

// Components
#include "ColliderComponent.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "SkinnedMeshComponent.h"
#include "TransformComponent.h"

// Services
#include "CameraService.h"
#include "EnvironmentService.h"
#include "LightService.h"
#include "ShaderService.h"
#include "TerrainService.h"

using namespace Angazi;

void Angazi::StaticMetaRegister()
{
	// Add MetaClass registration here...
	// eg. META_REGISTER(Foo);

	META_REGISTER(Component);
	META_REGISTER(GameObject);
	META_REGISTER(Service);

	META_REGISTER(ColliderComponent);
	META_REGISTER(MaterialComponent);
	META_REGISTER(MeshComponent);
	META_REGISTER(SkinnedMeshComponent);
	META_REGISTER(TransformComponent);

	META_REGISTER(CameraService);
	META_REGISTER(EnvironmentService);
	META_REGISTER(LightService);
	META_REGISTER(ShaderService);
	META_REGISTER(TerrainService);
}
