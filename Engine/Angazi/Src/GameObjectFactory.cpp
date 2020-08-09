#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "Component.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

using namespace Angazi;

GameObject* GameObjectFactory::Create(GameObjectAllocator & allocator, std::filesystem::path templateFileName)
{
	auto newObject = allocator.New();

	// TODO:
	// Construct the game object based on the data in the template file
	if (templateFileName == "../Assets/Templates/player.json")
	{
		// TODO:
		// Add transformComponent class , add setters/getters, and a function to return a Matrix4
		// Add ColliderComponent class, add setters/getters for an AABB, add Render to draw the aabb
		auto transform = newObject->AddComponent<TransformComponent>();
		transform->SetPosition({1.0f,2.0f,3.0f});

		auto collider = newObject->AddComponent<ColliderComponent>();
		collider->SetAABB({Math::Vector3::Zero,Math::Vector3::One});
	}

	return newObject;
}

void GameObjectFactory::Destory(GameObjectAllocator & allocator, GameObject * gameObject)
{
	allocator.Delete(gameObject);
}
