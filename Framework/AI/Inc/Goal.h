#pragma once

namespace Angazi::AI
{
	template <class AgentType>
	class Goal
	{
	public:
		enum class Status { Inactive, Active, Completed, Failed };

		Goal(AgentType& agent)
			: mAgent(agent)
		{}

		virtual ~Goal() = default;

		virtual void Activate() = 0;
		virtual Status Process() = 0;
		virtual void Terminate() = 0;

	protected:
		void ActivateIfInactive()
		{
			if (mStatus == Status::Inactive)
				Activate();
		}

		AgentType& mAgent;
		Status mStatus = Status::Inactive;
	};
}