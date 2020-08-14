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
	if (templateFileName == "tallBox")
	{
		// TODO:
		// Add transformComponent class , add setters/getters, and a function to return a Matrix4
		// Add ColliderComponent class, add setters/getters for an AABB, add Render to draw the aabb
		auto transform = newObject->AddComponent<TransformComponent>();
		transform->position = { -5.0f,0.0f,0.0f };

		auto collider = newObject->AddComponent<ColliderComponent>();
		collider->center = {0.0f, 3.0f, 0.0f};
		collider->extend = { 1.0f, 3.0f, 1.0f };
	}
	else if (templateFileName == "longBox")
	{
		// TODO:
		// Add transformComponent class , add setters/getters, and a function to return a Matrix4
		// Add ColliderComponent class, add setters/getters for an AABB, add Render to draw the aabb
		auto transform = newObject->AddComponent<TransformComponent>();
		transform->position = { 0.0f,0.0f,0.0f };

		auto collider = newObject->AddComponent<ColliderComponent>();
		collider->center = { 0.0f, 1.0f, 0.0f };
		collider->extend = { 1.0f, 1.0f, 4.0f };
	}
	else if (templateFileName == "fatBox")
	{
		// TODO:
		// Add transformComponent class , add setters/getters, and a function to return a Matrix4
		// Add ColliderComponent class, add setters/getters for an AABB, add Render to draw the aabb
		auto transform = newObject->AddComponent<TransformComponent>();
		transform->position = { 5.0f,0.0f,0.0f };

		auto collider = newObject->AddComponent<ColliderComponent>();
		collider->center = { 0.0f, 1.0f, 0.0f };
		collider->extend = { 3.0f, 1.0f, 1.0f };
	}

	return newObject;
}

void GameObjectFactory::Destory(GameObjectAllocator & allocator, GameObject * gameObject)
{
	allocator.Delete(gameObject);
}
