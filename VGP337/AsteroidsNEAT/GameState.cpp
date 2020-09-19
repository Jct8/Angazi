#include "GameState.h"
#include "ImGui/Inc/imgui.h"

#include "Ship.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

std::vector<Ship> ships;
std::unique_ptr<AI::NEAT::Population> population;
size_t activeShipCount = 0;
float bestFitness = 0.0f;
float previousBest = 0.0f;

namespace
{
	void Title(float deltaTime);
	void Play(float deltaTime);
	void RunNEAT(float deltaTime);
	std::function<void(float)> Tick = Title;

	void Title(float deltaTime)
	{
		BatchRender::Get()->AddScreenText("Hit [Space] to Play", 10.0f, 10.0f, 20.0f, Colors::White);
		BatchRender::Get()->AddScreenText("Hit [N] to NEAT", 10.0f, 30.0f, 20.0f, Colors::White);

		if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
		{
			auto& ship = ships.emplace_back();
			ship.Spawn();
			ship.Load();

			Tick = Play;
		}
		else if (InputSystem::Get()->IsKeyPressed(KeyCode::N))
		{
			population = std::make_unique<AI::NEAT::Population>(8, 4);

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
					auto& ship = ships.emplace_back();
					ship.Spawn();

					ship.brain = std::make_unique<AI::NEAT::NeuralNet>();
					ship.brain->Initialize(g, population->neuralNetConfig);
					ship.fitness = 0.0f;

					++activeShipCount;
				}
			}

			Tick = RunNEAT;
		}

	}

	void Play(float deltaTime)
	{
		if (!ships[0].IsAlive())
		{
			ships.clear();
			Tick = Title;
		}

		for (auto& ship : ships)
			ship.Update(deltaTime);

		for (auto& ship : ships)
			ship.Render();
	}

	void RunNEAT(float deltaTime)
	{
		if (activeShipCount == 0)
		{
			previousBest = FLT_MIN;
			// Feed bird fitness back into the genome
			for (auto& s : population->species)
			{
				for (auto& g : s.genomes)
				{
					if (ships[activeShipCount].fitness > bestFitness)
						bestFitness = ships[activeShipCount].fitness;
					if (ships[activeShipCount].fitness > previousBest)
						previousBest = ships[activeShipCount].fitness;
					g.fitness = static_cast<size_t>(ships[activeShipCount++].fitness);
				}
			}

			population->NewGeneration();

			// Use new species/genomes to respawn birds with new brains
			activeShipCount = 0;
			for (auto& s : population->species)
			{
				for (auto& g : s.genomes)
				{
					auto& ship = ships[activeShipCount++];
					ship.Spawn();

					ship.brain = std::make_unique<AI::NEAT::NeuralNet>();
					ship.brain->Initialize(g, population->neuralNetConfig);
					ship.fitness = 0.0f;
				}
			}
		}

		activeShipCount = 0;
		for (auto& ship : ships)
		{
			ship.Update(deltaTime);
			if (ship.IsAlive())
				++activeShipCount;
		}

		for (auto& ship : ships)
		{
			if (ship.IsAlive())
			{
				ship.Render();
				break;
			}
		}

		std::string txt;
		txt = "Generation: " + std::to_string(population->Generation());
		BatchRender::Get()->AddScreenText(txt.c_str(), 10.0f, 10.0f, 20.0f, Colors::White);

		txt = "Total Species:" + std::to_string(population->species.size());
		BatchRender::Get()->AddScreenText(txt.c_str(), 10.0f, 30.0f, 20.0f, Colors::White);

		txt = "Previous Generation Best Fitness: " + std::to_string(previousBest);
		BatchRender::Get()->AddScreenText(txt.c_str(), 10.0f, 50.0f, 20.0f, Colors::White);

		txt = "Best Fitness:" + std::to_string(bestFitness);
		BatchRender::Get()->AddScreenText(txt.c_str(), 10.0f, 70.0f, 20.0f, Colors::White);

		txt = "Total Ships Alive:" + std::to_string(activeShipCount);
		BatchRender::Get()->AddScreenText(txt.c_str(), 10.0f, 90.0f, 20.0f, Colors::White);

	}

}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
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
