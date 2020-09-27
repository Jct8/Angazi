#pragma once

namespace Angazi
{
	class GameWorld;

	class Service
	{
	public:
		META_CLASS_DECLARE
	public:
		Service() = default;
		virtual ~Service() = default;

		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}

		virtual void ShowInspectorProperties(){}

		GameWorld& GetWorld() { return *mWorld; }
		const GameWorld& GetWorld() const { return *mWorld; }

	private:
		friend class GameWorld;
		friend class Editor;

		GameWorld* mWorld = nullptr;
		bool mEnabled = true;
	};
}


