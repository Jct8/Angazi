#include <Angazi/Inc/Angazi.h>
#include <iostream>

using namespace Angazi;

int main()
{
	const std::string kTarget = "I love Genetic Algorithm!";
	const int validGeneValue = 126 - 32;

	Angazi::AI::GeneticAlgorithm ga;

	// Intial functions
	AI::GeneticAlgorithm::ComputeFitness computeFitness = [&kTarget](auto &genome)
	{
		genome.fitness = 0.0f;
		for (size_t i = 0; i < kTarget.size(); ++i)
		{
			if (kTarget[i] != genome.chromosome[i] + 32)
				genome.fitness += 1.0f; // penalize any characters that don't match the target
		}
	};
	AI::GeneticAlgorithm::CreateGenome createGenome = [&kTarget,validGeneValue]()
	{
		AI::GeneticAlgorithm::Genome genome;
		int chromoLength = static_cast<int>(kTarget.size());

		genome.chromosome.reserve(chromoLength);
		for (int i = 0; i < chromoLength; ++i)
			genome.chromosome.push_back(Math::RandomInt(0, validGeneValue));
		genome.fitness = 0.0f;
		return genome;
	};
	AI::GeneticAlgorithm::Crossover crossOver = [](auto& parent1, auto& parent2)
	{
		AI::GeneticAlgorithm::Genome offspring;
		float mCrossoverRate = 0.45f;

		for (size_t i = 0; i < parent1.chromosome.size(); ++i)
		{
			if (Math::RandomFloat() < mCrossoverRate)
				offspring.chromosome.push_back(parent1.chromosome[i]);
			else
				offspring.chromosome.push_back(parent2.chromosome[i]);
		}

		return offspring;
	};
	AI::GeneticAlgorithm::Mutation mutate = [validGeneValue](auto& genome)
	{
		float mMutationRate = 0.1f;

		for (size_t i = 0; i < genome.chromosome.size(); ++i)
		{
			if (Math::RandomFloat() < mMutationRate)
				genome.chromosome[i] = Math::RandomInt(0, validGeneValue);
		}
	};

	ga.Initialize(100, 0.0f, createGenome, crossOver, mutate, computeFitness);

	auto Print = [](const Angazi::AI::GeneticAlgorithm& ga)
	{
		int gen = ga.GetGeneration();
		auto& fittest = ga.BestGenome();
		std::string result;
		for (auto i : fittest.chromosome)
			result += (char)(i + 32);
		LOG("Generation: %d - Result: %s - Fitness: %f", gen, result.c_str(), fittest.fitness);
		std::cout << "Generation: "<< gen <<" - Result: "<< result.c_str() <<" - Fitness: "<< fittest.fitness << std::endl;
	};

	while (true)
	{
		ga.Advance();
		if (ga.Found())
			break;
		Print(ga);
	}
	Print(ga);

	return 0;
}
