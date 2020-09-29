#pragma once

namespace Angazi
{
	class GameObject;

	class Component
	{
	public:
		META_CLASS_DECLARE

		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;

		virtual ~Component() = default;
		Component() = default;

		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void Render() {}

		virtual void DebugUI() {}
		virtual void ShowInspectorProperties() {}

		GameObject& GetGameObject() { return *mOwner; }
		const GameObject& GetGameObject() const { return *mOwner; }

	protected:
		bool mInitialized = false;
	private:
		friend class GameObject;
		GameObject* mOwner = nullptr;
	};
}