#pragma once

#include "GameObjectFactory.h"

namespace Angazi
{
	class Service;
	class Editor;

	class GameWorld
	{
	public:
		void Initialize(size_t capacity);
		void Terminate();

		template<class ServiceType>
		ServiceType* AddService()
		{
			static_assert(std::is_base_of_v<Service,ServiceType>, "GameWorld -- 'ServiceType' must be derived from type 'Service'.");
			ASSERT(!mInitialized, "GameWorld -- cannot add service after world has already been initialized.");

			auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
			newService->mWorld = this;
			return static_cast<ServiceType*>(newService.get());
		}

		template <class ServiceType>
		const ServiceType* GetService() const
		{
			for (auto& service : mServices)
			{
				if (service->GetMetaClass() == ServiceType::StaticGetMetaClass())
					return static_cast<const ServiceType*>(service.get());
			}
			return nullptr;
		}

		template <class ServiceType>
		ServiceType* GetService()
		{
			const GameWorld* constMe = static_cast<const GameWorld*>(this);
			return const_cast<ServiceType*>(constMe->GetService<ServiceType>());
		}

		GameObjectHandle Create(const std::filesystem::path& templateFileName, std::string name);
		GameObjectHandle CreateEmpty();
		GameObjectHandle Find(const std::string& name);

		void LoadScene(const std::filesystem::path& sceneFileName);
		void UnloadScene();
		void SaveScene(const std::filesystem::path& sceneFilePath = "");

		void Destroy(GameObjectHandle handle);

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		void RenderShadowMap();

	private:
		friend Editor;
		void DestroyInternal(GameObject* gameObject); 
		void ProcessDestroyList();

		using Services = std::vector<std::unique_ptr<Service>>;
		using GameObjectList = std::vector<GameObject*>;

		Services mServices;

		std::unique_ptr<GameObjectAllocator> mGameObjectAllocator;
		std::unique_ptr<GameObjectHandlePool> mGameObjectHandlePool;

		GameObjectList mUpdateList;
		GameObjectList mDestroyList;

		std::filesystem::path mSceneFilePath = "";

		bool mInitialized = false;
		bool mUpdating = false;
	};
}

