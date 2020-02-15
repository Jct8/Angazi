#pragma once

namespace Angazi::AI
{
	class AIWorld;

	class Entity
	{
	public:
		Entity(AIWorld& world, int typeId);
		virtual ~Entity();

		Angazi::Math::Matrix3 LocalToWorld() const;

		AIWorld& world;
		Angazi::Math::Vector2 position = Angazi::Math::Vector2::Zero;
		Angazi::Math::Vector2 heading =  Angazi::Math::Vector2::YAxis;

		const int id = 0;
		float radius = 1.0f;

	};

	using Entities = std::vector<Entity*>;

}