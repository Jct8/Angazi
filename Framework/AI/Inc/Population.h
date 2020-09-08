#pragma once

#include "Config.h"
#include "Genome.h"
#include "InnovationContainer.h"
#include "Species.h"

namespace Angazi::AI::NEAT
{
	class Population
	{
	public:
		MutationConfig mutationConfig;
		SpeciatingConfig speciatingConfig;
		NeuralNetConfig networkConfig;

	private:
		// Evolutionary methods
		Genome crossover(const Genome& g1, const Genome& g2);
		void Mutate_weight(Genome& g);
		void Mutate_enable_disable(Genome& g, bool enable);
		void Mutate_link(Genome& g, bool force_bias);
		void Mutate_node(Genome& g);
		void Mutate(Genome& g);

		double Disjoint(const Genome& g1, const Genome& g2);
		double Weights(const Genome& g1, const Genome& g2);
		bool IsSameSpecies(const Genome& g1, const Genome& g2);

		// Species Ranking
		void RankGlobally();
		void CalculateAverageFitness(Species& s);
		size_t TotalAverageFitness();

		// Evolution
		void CullSpecies(bool cut_to_one);
		Genome BreedChild(Species& s);
		void RemoveStaleSpecies();
		void RemoveWeakSpecies();
		void AddToSpecies(Genome& child);

		InnovationContainer mInnovation;

		size_t generation_number = 1;
	};
}