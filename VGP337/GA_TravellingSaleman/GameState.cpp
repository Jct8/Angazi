#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	RestartAlgorithm();
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	if (mCurrentDelay < Core::TimeUtil::GetTime() && !mGeneticAlgorithm.Found())
	{
		mGeneticAlgorithm.Advance();
		mCurrentDelay = Core::TimeUtil::GetTime() + mDelay;
	}
}

void GameState::Render()
{
	for (auto city : mCities)
		SimpleDraw::AddScreenCircle({ city , 20.0f }, Colors::AliceBlue);

	// Connect Cities
	auto& fittest = mGeneticAlgorithm.BestGenome();
	for (size_t i = 0; i < fittest.chromosome.size(); i++)
	{
		int nextIndex = (i + 1) % fittest.chromosome.size();
		SimpleDraw::AddScreenLine(mCities[fittest.chromosome[i]], mCities[fittest.chromosome[nextIndex]], Colors::AliceBlue);
	}

	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	int gen = mGeneticAlgorithm.GetGeneration();
	auto& fittest = mGeneticAlgorithm.BestGenome();
	ImGui::Begin("Genetic Algorithm", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.2f", Angazi::Core::TimeUtil::GetFramesPerSecond());
	ImGui::NewLine();
	

	ImGui::Text("Current Best Genome:");
	ImGui::Text("Generation: %d - Fitness : %.2f", gen, fittest.fitness);
	ImGui::Text("Target fitness: %.2f", threshold);
	ImGui::SliderFloat("Generation delay", &mDelay, 0.0f, 2.0f);
	ImGui::SliderFloat("Mutation Rate", &mutationRate, 0.1f, 1.0f);
	ImGui::NewLine();
	
	ImGui::SliderInt("Number of Cities", &numCities, 6, 50);
	ImGui::SliderInt("Genome Population", &mPopulation, 1, 200);
	if (ImGui::Button("Restart"))
		RestartAlgorithm();
	ImGui::End();
}

void GameState::RestartAlgorithm()
{
	float increment = Constants::TwoPi / numCities;
	float theta = 0.0f;
	float radius = 300.0f;
	float width = 0.5f * GraphicsSystem::Get()->GetBackBufferWidth();
	float height = 0.5f * GraphicsSystem::Get()->GetBackBufferHeight();

	mCities.clear();
	for (int i = 0; i < numCities; i++)
	{
		float x = cos(theta)*radius + width;
		float y = sin(theta)*radius + height;
		mCities.push_back({ x,y });
		theta += increment;
	}

	std::random_device rd;
	std::mt19937 g(rd());

	AI::GeneticAlgorithm::CreateGenome createGnome = [this, &g]()
	{
		AI::GeneticAlgorithm::Genome genome;
		int chromoLength = static_cast<int>(numCities);

		genome.chromosome.reserve(chromoLength);
		for (int i = 0; i < chromoLength; ++i)
			genome.chromosome.push_back(i);
		std::shuffle(genome.chromosome.begin(), genome.chromosome.end(),g);
		genome.fitness = 0.0f;
		return genome;
	};
	AI::GeneticAlgorithm::ComputeFitness computeFitness = [this](AI::GeneticAlgorithm::Genome& genome)
	{
		genome.fitness = 0.0f;
		for (size_t i = 0; i < genome.chromosome.size(); i++)
		{
			int nextIndex = (i + 1) % genome.chromosome.size();
			genome.fitness += Distance(mCities[genome.chromosome[i]], mCities[genome.chromosome[nextIndex]]);
		}
	};
	AI::GeneticAlgorithm::Crossover crossOver = [this](auto& parent1, auto& parent2)
	{
		AI::GeneticAlgorithm::Genome offspring;
		offspring.chromosome = parent1.chromosome;

		int maxCities = (int)mCities.size() - 1;
		int crossoverIndex = RandomInt(0, maxCities);
		std::unordered_map<int, int> crossoverMap;

		for (size_t i = crossoverIndex; i < parent2.chromosome.size(); ++i)
			crossoverMap[parent1.chromosome[i]] = parent2.chromosome[i];

		// Apply Crossover
		for (auto map : crossoverMap)
		{
			auto firstIter = std::find(offspring.chromosome.begin(), offspring.chromosome.end(), map.first);
			auto secondIter = std::find(offspring.chromosome.begin(), offspring.chromosome.end(), map.second);

			std::iter_swap(firstIter, secondIter);
		}

		return offspring;
	};
	AI::GeneticAlgorithm::Mutation mutate = [this](auto& genome)
	{
		if (Math::RandomFloat() < mutationRate)
		{
			int maxCities = (int)mCities.size() - 1;
			int randomCity = Math::RandomInt(0, maxCities);
			int randomIndex = Math::RandomInt(0, maxCities);
			auto firstIter = std::find(genome.chromosome.begin(), genome.chromosome.end(), randomCity);
			std::iter_swap(firstIter, genome.chromosome.begin() + randomIndex);
		}
	};

	threshold = 2.0f * Constants::Pi * radius;
	mGeneticAlgorithm.Initialize(mPopulation, threshold, createGnome, crossOver, mutate, computeFitness);
}
