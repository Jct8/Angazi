#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "Component.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

using namespace Angazi;

GameObject* GameObjectFactory::Create(GameObjectAllocator & allocator, std::filesystem::path templateFileName)
{
	auto newObject = allocator.New();

	FILE *file = nullptr;
	fopen_s(&file, templateFileName.u8string().c_str(), "r");

	char readBuffer[65536];
	rapidjson::FileReadStream is(file, readBuffer, sizeof(readBuffer));

	rapidjson::Document document;
	document.ParseStream(is);

	if (document.HasMember("GameObject"))
	{
		if (document["GameObject"].HasMember("Components"))
		{
			auto components = document["GameObject"]["Components"].GetObjectW();
			if (components.HasMember("TransformComponent"))
			{
				auto transform = newObject->AddComponent<TransformComponent>();
				if (components["TransformComponent"].HasMember("Position"))
				{
					auto x = components["TransformComponent"]["Position"]["x"].GetFloat();
					auto y = components["TransformComponent"]["Position"]["y"].GetFloat();
					auto z = components["TransformComponent"]["Position"]["z"].GetFloat();
					transform->position = { x , y, z };
				}
				if (components["TransformComponent"].HasMember("Position"))
				{
					auto x = components["TransformComponent"]["Rotation"]["x"].GetFloat();
					auto y = components["TransformComponent"]["Rotation"]["y"].GetFloat();
					auto z = components["TransformComponent"]["Rotation"]["z"].GetFloat();
					auto w = components["TransformComponent"]["Rotation"]["w"].GetFloat();
					transform->rotation = { x , y, z, w };
				}
			}
			if (document["GameObject"]["Components"].HasMember("ColliderComponent"))
			{
				auto collider = newObject->AddComponent<ColliderComponent>();
				if (components["ColliderComponent"].HasMember("Center"))
				{
					auto x = components["ColliderComponent"]["Center"]["x"].GetFloat();
					auto y = components["ColliderComponent"]["Center"]["y"].GetFloat();
					auto z = components["ColliderComponent"]["Center"]["z"].GetFloat();
					collider->center = { x , y, z };
				}
				if (components["ColliderComponent"].HasMember("Extend"))
				{
					auto x = components["ColliderComponent"]["Extend"]["x"].GetFloat();
					auto y = components["ColliderComponent"]["Extend"]["y"].GetFloat();
					auto z = components["ColliderComponent"]["Extend"]["z"].GetFloat();
					collider->extend = { x , y, z };
				}
			}

		}
	}
	return newObject;
}

void GameObjectFactory::Destory(GameObjectAllocator & allocator, GameObject * gameObject)
{
	allocator.Delete(gameObject);
}
