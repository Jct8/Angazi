#pragma once

namespace Angazi
{
	class Component;
	class GameObject;
	class GameWorld;

	using GameObjectAllocator = Core::TypedAllocator<GameObject>;
	using GameObjectHandle = Core::Handle<GameObject>;
	using GameObjectHandlePool = Core::HandlePool<GameObject>;

	class GameObject final
	{
	public:
		GameObject() = default;
		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;

		virtual void Initialize();
		virtual void Terminate();

		virtual void Update(float deltaTime);
		virtual void Render();
		virtual void DebugUI();

		//template <class T, class = std::void_t<std::is_base_of<Component, T>>>
		//template <class T, class = std::void_t<std::enable_if_t<std::is_base_of_v<Component, T>>>>
		template <class ComponentType>
		ComponentType* AddComponent()
		{
			static_assert(std::is_base_of_v<Component, ComponentType>,"GameObject -- Cannot add type T which is not derived from Component.");
			ASSERT(!mInitialized, " GameObject -- Cannot Add new components once the game object is already initialized.");

			auto& newComponent = mComponents.emplace_back(std::make_unique<ComponentType>());
			newComponent->mOwner = this;
			return static_cast<ComponentType*>(newComponent.get());
		}

		template <class ComponentType>
		const ComponentType* GetComponent() const
		{
			for (auto& component : mComponents)
			{
				if (component->GetMetaClass() == Component::StaticGetMetaClass())
					return static_cast<const ComponentType*>(component.get());
			}
			return nullptr;
		}

		template <class ComponentType>
		ComponentType* GetComponent()
		{
			const GameObject* constMe = static_cast<const GameObject*>(this);
			return const_cast<ComponentType*>(constMe->GetComponent<ComponentType>());
		}

		GameWorld& GetWorld() { return *mWorld; }
		const GameWorld& GetWorld() const { return *mWorld; }

		void SetName(const char* name) { mName = name; }
		const std::string& GetName() const { return mName ; }

		GameObjectHandle GetHandle() const { return mHandle; }
	private:
		friend class GameWorld;

		using Components = std::vector<std::unique_ptr<Component>>;

		GameWorld* mWorld = nullptr;
		std::string mName = "NoName";
		GameObjectHandle mHandle;
		Components mComponents;
		bool mInitialized = false;
	};
}