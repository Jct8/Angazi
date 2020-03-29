#pragma once

namespace Angazi::AI
{
	class AIWorld;
	class Entity;

	using EntityList = std::vector<Entity*>;

	class Entity
	{
	public:
		Entity(AIWorld& world, uint32_t typeId);
		virtual ~Entity();

		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;
		Entity(const Entity&&) = delete;
		Entity& operator=(const Entity&&) = delete;

		Math::Matrix3 LocalToWorld() const;

		const uint32_t GetTypeId() const { return mUniqueId >> 32; }
		const uint64_t  GetUniqueId() const { return mUniqueId; }

		AIWorld& world;
		Math::Vector2 position = Math::Vector2::Zero;
		Math::Vector2 heading = Math::Vector2::YAxis;

		float radius = 1.0f;

	private:
		const uint64_t mUniqueId = 0;
	};
}