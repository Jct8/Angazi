#include "GameState.h"
#include "ImGui/Inc/imgui.h"

#include "Background.h"
#include "Bird.h"
#include "PipeManager.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

Background bg;
PipeManager pm;
std::vector<Bird> birds;
std::unique_ptr<AI::NEAT::Population> population;
size_t activeBirdCount = 0;
float bestFitness = 0;

namespace
{
	void Title(float deltaTime);
	void Play(float deltaTime);
	void RunNEAT(float deltaTime);
	bool RunXOR(float deltaTime);
	std::function<void(float)> Tick = Title;

	void Title(float deltaTime)
	{
		bg.Render();
		BatchRender::Get()->AddScreenText("Hit [Space] to Flap", 10.0f, 10.0f, 20.0f, Colors::White);
		BatchRender::Get()->AddScreenText("Hit [N] to NEAT", 10.0f, 30.0f, 20.0f, Colors::White);
		BatchRender::Get()->AddScreenText("Hit [X] to XOR Test", 10.0f, 50.0f, 20.0f, Colors::White);

		if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
		{
			auto& bird = birds.emplace_back();
			bird.Load();
			bird.Spawn({ 150.0f, 300.0f });

			Tick = Play;
		}
		else if (InputSystem::Get()->IsKeyPressed(KeyCode::N))
		{
			population = std::make_unique<AI::NEAT::Population>(4, 1);

			AI::NEAT::MutationConfig& mutationConfig = population->mutationConfig;
			mutationConfig.connection_mutate_chance = 0.55;
			mutationConfig.perturb_chance = 0.55;
			mutationConfig.crossover_chance = 0.5;
			mutationConfig.link_mutation_chance = 0.65;
			mutationConfig.node_mutation_chance = 0.45;
			mutationConfig.bias_mutation_chance = 0.2;
			mutationConfig.step_size = 0.2;
			mutationConfig.disable_mutation_chance = 0.3;
			mutationConfig.enable_mutation_chance = 0.3;

			for (auto& s : population->species)
			{
				for (auto& g : s.genomes)
				{
					auto& bird = birds.emplace_back();
					bird.Load();
					bird.Spawn({ 150.0f, 300.0f });

					bird.brain = std::make_unique<AI::NEAT::NeuralNet>();
					bird.brain->Initialize(g, population->neuralNetConfig);
					bird.fitness = 0.0f;

					++activeBirdCount;
				}
			}

			Tick = RunNEAT;
		}
		else if (InputSystem::Get()->IsKeyPressed(KeyCode::X))
		{
			Tick = RunXOR;
		}
	}

	void Play(float deltaTime)
	{
		if (!birds[0].IsAlive())
		{
			birds.clear();
			pm.Reset();
			Tick = Title;
		}

		bg.Update(deltaTime);
		pm.Update(deltaTime);

		for (auto& bird : birds)
			bird.Update(deltaTime, pm);

		if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
			birds[0].Flap();

		bg.Render();
		pm.Render();
		for (auto& bird : birds)
			bird.Render();
	}

	void RunNEAT(float deltaTime)
	{
		if (activeBirdCount == 0)
		{
			// Feed bird fitness back into the genome
			for (auto& s : population->species)
				for (auto& g : s.genomes)
				{
					if (birds[activeBirdCount].fitness > bestFitness)
						bestFitness = birds[activeBirdCount].fitness;
					g.fitness = static_cast<size_t>(birds[activeBirdCount++].fitness);
				}

			population->NewGeneration();

			// Use new species/genomes to respawn birds with new brains
			activeBirdCount = 0;
			for (auto& s : population->species)
			{
				for (auto& g : s.genomes)
				{
					auto& bird = birds[activeBirdCount++];
					bird.Spawn({ 150.0f, 300.0f });

					bird.brain = std::make_unique<AI::NEAT::NeuralNet>();
					bird.brain->Initialize(g, population->neuralNetConfig);
					bird.fitness = 0.0f;
				}
			}

			pm.Reset();
		}

		bg.Update(deltaTime);
		pm.Update(deltaTime);

		activeBirdCount = 0;
		for (auto& bird : birds)
		{
			bird.Update(deltaTime, pm);
			if (bird.IsAlive())
				++activeBirdCount;
		}

		bg.Render();
		pm.Render();
		for (auto& bird : birds)
			bird.Render();

		std::string txt;
		txt += "Generation: " + std::to_string(population->Generation());
		BatchRender::Get()->AddScreenText(txt.c_str(), 10.0f, 10.0f, 20.0f, Colors::White);

		txt = "Best Fitness:" + std::to_string(bestFitness);
		BatchRender::Get()->AddScreenText(txt.c_str(), 10.0f, 30.0f, 20.0f, Colors::White);

		txt = "Total Birds:" + std::to_string(activeBirdCount);
		BatchRender::Get()->AddScreenText(txt.c_str(), 10.0f, 50.0f, 20.0f, Colors::White);
	}

	bool RunXOR(float deltaTime)
	{
		auto XORTest = [](AI::NEAT::NeuralNet& n, bool log) -> size_t
		{
			size_t fitness = 0;

			std::string message = "     > begin xor test\n        ( ";

			auto output = n.Evaluate({ 0.0, 0.0 });
			message += std::to_string(output[0]) + " ";
			fitness += (size_t)std::min(1.0 / ((0.0 - output[0]) * (0.0f - output[0])), 50.0);

			output = n.Evaluate({ 0.0, 1.0 });
			message += std::to_string(output[0]) + " ";
			fitness += (size_t)std::min(1.0 / ((1.0 - output[0]) * (1.0 - output[0])), 50.0);

			output = n.Evaluate({ 1.0, 0.0 });
			message += std::to_string(output[0]) + " ";
			fitness += (size_t)std::min(1.0 / ((1.0 - output[0]) * (1.0 - output[0])), 50.0);

			output = n.Evaluate({ 1.0, 1.0 });
			message += std::to_string(output[0]) + " ";
			fitness += (size_t)std::min(1.0 / ((0.0 - output[0]) * (0.0 - output[0])), 50.0);

			if (log)
			{
				message += ") fitness = " + std::to_string(fitness);
				LOG("%s", message.c_str());
			}

			return fitness;
		};

		AI::NEAT::Population p(2, 1);
		AI::NEAT::NeuralNet bestGuy;

		AI::NEAT::MutationConfig& mutationConfig = p.mutationConfig;
		mutationConfig.connection_mutate_chance = 0.65;
		mutationConfig.perturb_chance = 0.9;
		mutationConfig.crossover_chance = 0.75;
		mutationConfig.link_mutation_chance = 0.85;
		mutationConfig.node_mutation_chance = 0.5;
		mutationConfig.bias_mutation_chance = 0.2;
		mutationConfig.step_size = 0.1;
		mutationConfig.disable_mutation_chance = 0.2;
		mutationConfig.enable_mutation_chance = 0.2;

		size_t max_fitness = 0;
		while (max_fitness < 200)
		{
			size_t current_fitness = 0;
			size_t min_fitness = 100000;
			for (auto& s : p.species)
			{
				for (auto& g : s.genomes)
				{
					AI::NEAT::NeuralNet n;
					n.Initialize(g, p.neuralNetConfig);
					current_fitness = XORTest(n, false);
					if (current_fitness < min_fitness)
						min_fitness = current_fitness;
					if (current_fitness > max_fitness)
					{
						max_fitness = current_fitness;
						bestGuy = n;
					}
					g.fitness = current_fitness;
				}
			}

			LOG("Generation %zd successfuly tested. Species: %zd, Global min fitness: %zd, Global max fitness: %zd",
				p.Generation(), p.species.size(), min_fitness, max_fitness);
			p.NewGeneration();
		}

		XORTest(bestGuy, true);
		return true;
	}
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	bg.Load();
}

void GameState::Terminate()
{
}

void GameState::Update(float deltaTime)
{
	Tick(deltaTime);
}

void GameState::Render()
{
}

void GameState::DebugUI()
{
	SimpleDraw::Render(mCamera);
}
