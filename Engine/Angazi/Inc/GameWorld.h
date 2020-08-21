#pragma once

#include "GameObjectFactory.h"

namespace Angazi
{
	class Service;

	class GameWorld
	{
	public:
		void Initialize(size_t capacity);
		void Terminate();

		template<class ServiceType>
		ServiceType* AddService()
		{
			static_assert(std::is_base_of_v<Service>, "GameWorld -- 'ServiceType' must be derived from type 'Service'.");
			ASSERT(!mInitialized, "GameWorld -- cannot add service after world has already been initialized.");

			auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
			newService->mWorld = this;
			return static_cast<ServiceType*>(newService.get());
		}

		template<class ServiceType>
		ServiceType* GetService()
		{
			// Hack - assume the first service is the service we want.
			auto iter = mServices.begin();
			return static_cast<ServiceType*>(iter->get());
		}


		GameObjectHandle Create(const std::filesystem::path& templateFileName, std::string name);
		GameObjectHandle Find(const std::string& name);
		void Destroy(GameObjectHandle handle);

		void Update(float deltaTime);
		void Render();
		void DebugUI();

	private:
		void DestroyInternal(GameObject* gameObject); 
		void ProcessDestroyList();

		using Services = std::vector<std::unique_ptr<Service>>;
		using GameObjectList = std::vector<GameObject*>;

		Services mServices;

		std::unique_ptr<GameObjectAllocator> mGameObjectAllocator;
		std::unique_ptr<GameObjectHandlePool> mGameObjectHandlePool;

		GameObjectList mUpdateList;
		GameObjectList mDestroyList;

		bool mInitialized = false;
		bool mUpdating = false;
	};
}

