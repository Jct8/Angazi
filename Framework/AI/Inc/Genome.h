#pragma once

#include "Common.h"

namespace Angazi::AI::NEAT
{
	struct Gene
	{
		size_t innovationNum = -1;
		size_t fromNode = -1;
		size_t toNode = -1;
		double weight = 0.0;
		bool enabled = true;
	} ;

	struct Genome
	{
		size_t fitness = 0;
		size_t adjustedFitness = 0;
		size_t globalRank = 0;
		size_t maxNeuron;

		std::map<size_t, Gene> genes;

		Genome(size_t functional_nodes) 
			:maxNeuron(functional_nodes)
		{
		}

	};
}
