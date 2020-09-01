#pragma once

namespace Angazi::Network
{
	// A helper clas that pairs pointers with ids so that
	// we can refer to object instance across a network.
	class LinkingContext
	{
	public:
		static constexpr int InvalidId = -1;
	public:
		int GetNetworkId(void* instance) const;
		void* GetInstance(int networkId) const;

		// Register an instance to the next network id
		int Register(void* instance);

		// Register an instance to a specific network id
		void Register(void* instance, int networkId, bool updateNextId = false);

		// Unregister an instance
		void Unregister(void* instance);

	private:
		std::unordered_map<int, void*> mNetworkIdToInstanceMap;
		std::unordered_map<void*, int> mInstanceToNetworkIdMap;
		int mNextNetworkId = 0;
	};

}
