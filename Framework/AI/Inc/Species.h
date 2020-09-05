#pragma once

#include "Genome.h"

namespace Angazi::AI::NEAT
{
	struct Species
	{
		size_t top_fitness = 0;
		size_t average_fitness = 0;
		size_t staleness = 0;

		std::vector<Genome> genomes;
	};
}