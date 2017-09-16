#pragma once
#include "../Neural/NeuralNetwork.h"

namespace TicTacToeGame
{
	class IPlayer;
	class NeuralNetworkPlayer;
}

namespace Genetic
{

	class GeneticLearning
	{
	public:
		GeneticLearning(const size_t* networkLayerSizes, const size_t networkLayerCount);
		~GeneticLearning();

		void Initialize();
		void NextGeneration();
		void ScoreGeneration();

		int GetCurrentGenerationCounter() const;
		const int* GetScores() const;
	protected:
		void CreateNetworks();
		void MutateCurrentPopulation();
		void Crossover(const Neural::BasicNeuralNetwork& parent1, const Neural::BasicNeuralNetwork& parent2, Neural::BasicNeuralNetwork* child);

	public:

		class TicTacToeGame::IPlayer* opponentToTestAgainst;
		class TicTacToeGame::NeuralNetworkPlayer* thisPlayer;

		size_t gamesPlayedPerScoring = 10;
		size_t populationSize = 16;
		float mutationRate = 0.015f;
		float survivalRate = 0.50f;
		bool elitism = true;

	protected:
		const size_t* networkLayerSizes;
		size_t networkLayerCount;
		
		size_t generationCounter = 0;

		Neural::BasicNeuralNetwork* currentGeneration;
		Neural::BasicNeuralNetwork* previousGeneration;
		
		int* scores;
	private:
		size_t FindFittestInCurrentGeneration(int*& scores);
	};

}

