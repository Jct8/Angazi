#include "Precompiled.h"
#include "GeneticAlgorithm.h"

using namespace Angazi;
using namespace Angazi::AI;

void GeneticAlgorithm::Initialize(int populationSize, float fitnessThreshold, CreateGenome createGenome, Crossover crossover, Mutation mutation, ComputeFitness computeFitness)
{
	mComputeFitness = std::move(computeFitness);
	mMutate = std::move(mutation);
	mCrossover = std::move(crossover);

	// Reset
	mGeneration = 0;
	mFound = false;
	mFitnessThreshold = fitnessThreshold;

	// Create initial population
	mPopulation.clear();
	mPopulation.reserve(populationSize);
	for (int i = 0; i < populationSize; ++i )
		mPopulation.push_back(createGenome());
	SortPopulation();
}

void GeneticAlgorithm::Advance()
{
	if (mFound)
		return;

	++mGeneration;

	std::vector<Genome> newGeneration;
	newGeneration.reserve(mPopulation.size());

	// Perform Elitism: keep top 10% , mate he rest using top 50% 
	const int eliteCount = ((int)mPopulation.size() * 10) / 100;
	for (int i = 0; i < eliteCount; ++i)
		newGeneration.push_back(mPopulation[i]);

	const int mateCount = (int)mPopulation.size() - eliteCount;
	const int cutoff = (int)mPopulation.size() / 2;
	for (int i = 0; i < mateCount; ++i)
	{
		const auto& parent1 = mPopulation[Math::RandomInt(0, cutoff)];
		const auto& parent2 = mPopulation[Math::RandomInt(0, cutoff)];
		newGeneration.push_back(Mate(parent1, parent2));
	}

	mPopulation = std::move(newGeneration);
	SortPopulation();

	if (mPopulation[0].fitness <= mFitnessThreshold)
		mFound = true;
}

void GeneticAlgorithm::SortPopulation()
{
	for (auto &genome : mPopulation)
		mComputeFitness(genome);
	std::sort(mPopulation.begin(), mPopulation.end(), [](const auto& a, const auto& b) {return a.fitness < b.fitness; });
}

GeneticAlgorithm::Genome GeneticAlgorithm::Mate(const Genome & parent1, const Genome & parent2)
{
	Genome offspring;
	offspring = mCrossover(parent1, parent2);
	mMutate(offspring);
	return offspring;
}
