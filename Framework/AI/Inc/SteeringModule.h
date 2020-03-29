#pragma once

#include "SteeringBehavior.h"

namespace Angazi::AI
{
	class SteeringModule
	{
	public:
		SteeringModule(Agent& agent);

		template <class BehaviorType>
		BehaviorType* AddBehavior(std::string name)
		{
			auto[iter, success] = mBehaviors.try_emplace(std::move(name), std::make_unique<BehaviorType>());
			return success ? static_cast<BehaviorType*>(iter->second.get()) : nullptr;
		}

		template <class BehaviorType>
		BehaviorType* GetBehavior(const std::string& name)
		{
			auto iter = mBehaviors.find(name);
			return iter != mBehaviors.end() ? static_cast<BehaviorType*>(iter->second.get()) : nullptr;
		}

		Math::Vector2 Calculate();
		void DebugDraw();

	private:
		using BehaviorMap = std::unordered_map<std::string, std::unique_ptr<SteeringBehavior>>;

		Agent& mAgent;
		BehaviorMap mBehaviors;
	};
}