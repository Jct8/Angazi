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
		unsigned int fitness = 0;
		unsigned int adjustedFitness = 0;
		unsigned int globalRank = 0;
		unsigned int maxNeuron;

		std::map<size_t, Gene> genes;

		Genome(size_t functional_nodes) 
			:maxNeuron(functional_nodes)
		{
		}

	};
}
