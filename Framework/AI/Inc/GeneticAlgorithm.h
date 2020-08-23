#pragma once

namespace Angazi::AI
{
	class GeneticAlgorithm
	{
	public:
		struct Genome
		{
			std::vector<int> chromosome;	// [gene][gene][gene][gene] ...
			float fitness = 0.0f;			// lower the better
		};

		using CreateGenome = std::function<Genome(void)>;
		using Mutation = std::function<void(Genome&)>;
		using Crossover = std::function<Genome(const Genome&, const Genome&)>;
		using ComputeFitness = std::function<void(Genome&)>;

		// Randomly generate the initial population
		void Initialize(int populationSize, float fitnessThreshold, CreateGenome createGenome, Crossover crossover, Mutation mutation, ComputeFitness computeFitness);

		// Apply crossover and mutation to produce the next generation
		void Advance();

		// Accessors
		const Genome& BestGenome() const { return mPopulation.front(); }
		int GetGeneration() const{ return mGeneration; }
		bool Found() const { return mFound; }

	private:
		void SortPopulation();
		Genome Mate(const Genome& parent1, const Genome& parent2);

	private:
		ComputeFitness mComputeFitness;
		Mutation mMutate;
		Crossover mCrossover;

		std::vector<Genome> mPopulation;

		float mFitnessThreshold = 0.0f;
		int mGeneration = 0;
		bool mFound = false;
	};
}