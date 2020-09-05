#include "Precompiled.h"
#include "InnovationContainer.h"

using namespace Angazi::AI::NEAT;

void InnovationContainer::Reset()
{
	mHistory.clear();
}

size_t Angazi::AI::NEAT::InnovationContainer::AddGene(const Gene & gene)
{
	auto pair = std::make_pair(gene.fromNode, gene.toNode);
	auto it = mHistory.find(pair);
	if (it == mHistory.end())
		return mHistory[pair] = ++mInnovatioNumber;
	else
		return (*it).second;
}
