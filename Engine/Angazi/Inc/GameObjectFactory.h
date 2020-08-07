#pragma once
#include "GameObject.h"

namespace Angazi::GameObjectFactory
{
	GameObject* Create(GameObjectAllocator& allocator, std::filesystem::path templateFileName);
	void Destory(GameObjectAllocator& allocator, GameObject* gameObject);
}