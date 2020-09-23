// ===============================================================
// Creator : Justin Tim
// Angazi Engine
// ===============================================================

#pragma once

#include "Common.h"

// App headers
#include "App.h"
#include "AppState.h"

// Components
#include "ColliderComponent.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "SkinnedMeshComponent.h"
#include "TransformComponent.h"

// Service headers
#include "CameraService.h"
#include "EnvironmentService.h"
#include "Service.h"

// Terrain headers
#include "Terrain.h"

// World headers
#include "Component.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameWorld.h"

// Editor
#include "Editor.h"

namespace Angazi 
{
	App& MainApp(); 
}