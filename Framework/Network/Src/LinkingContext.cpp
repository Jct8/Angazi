#include "Precompiled.h"
#include "LinkingContext.h"

using namespace Angazi::Network;

int LinkingContext::GetNetworkId(void * instance) const
{
	auto iter = mInstanceToNetworkIdMap.find(instance);
	return iter == mInstanceToNetworkIdMap.end() ? InvalidId : iter->second;
}

void* LinkingContext::GetInstance(int networkId) const
{
	auto iter = mNetworkIdToInstanceMap.find(networkId);
	return iter == mNetworkIdToInstanceMap.end() ? nullptr : iter->second;
}

int LinkingContext::Register(void * instance)
{
	auto id = GetNetworkId(instance);
	ASSERT(id == InvalidId, "Linking Conext - Instance already registered!");

	id = mNextNetworkId++;
	mInstanceToNetworkIdMap.emplace(instance, id);
	mNetworkIdToInstanceMap.emplace(id, instance);
	return id;
}

void LinkingContext::Register(void * instance, int networkId, bool updateNextId)
{
	auto id = GetNetworkId(instance);
	ASSERT(id == InvalidId, "Linking Conext - Instance already registered!");

	mInstanceToNetworkIdMap.emplace(instance, networkId);
	mNetworkIdToInstanceMap.emplace(networkId, instance);

	if (updateNextId)
		mNextNetworkId = networkId + 1;
}

void LinkingContext::Unregister(void * instance)
{
	auto id = GetNetworkId(instance);
	if (id != InvalidId)
	{
		mInstanceToNetworkIdMap.erase(instance);
		mNetworkIdToInstanceMap.erase(id);
	}
}
