#include "GeneticLearning.h"
#include "../Game/IPlayer.h"
#include "../Game/TicTacToe.h"
#include "../Neural/NetworkHelpers.h"
#include "../Players/NeuralNetworkPlayer.h"
#include "../Players/RandomPlayer.h"
#include "../Utility/Algorithms.h"


namespace Genetic
{

	GeneticLearning::GeneticLearning(const size_t * networkLayerSizes, const size_t networkLayerCount)
	{
		this->networkLayerSizes = networkLayerSizes;
		this->networkLayerCount = networkLayerCount;
	}

	GeneticLearning::~GeneticLearning()
	{
		delete[] previousGeneration;
		delete[] currentGeneration;
		delete[] scores;

		delete opponentToTestAgainst;
		delete thisPlayer;
	}

	void GeneticLearning::Initialize()
	{
		if (populationSize <= 3)
		{
			throw std::exception("Population size invalid. Algorithm can't work with that.");
		}

		CreateNetworks();
		scores = new int[populationSize];

		thisPlayer = new TicTacToeGame::NeuralNetworkPlayer(&currentGeneration[0]);
		opponentToTestAgainst = new TicTacToeGame::RandomPlayer();
	}

	void GeneticLearning::NextGeneration()
	{
		ScoreGeneration();

		size_t fittestIndividualIndex = FindFittestInCurrentGeneration(scores);

		//swap buffer pointers
		{
			Neural::BasicNeuralNetwork* temp = currentGeneration;
			currentGeneration = previousGeneration;
			previousGeneration = temp;
		}

		if (elitism)
		{
			//save best individual
			currentGeneration[0] = previousGeneration[fittestIndividualIndex];
		}
		const size_t startIndex = elitism ? 1 : 0;

		//procreation for science
		for (size_t i = startIndex; i < populationSize; i++)
		{
			Neural::BasicNeuralNetwork& parent1 = previousGeneration[rand() % populationSize];
			Neural::BasicNeuralNetwork& parent2 = previousGeneration[rand() % populationSize];

			Crossover(parent1, parent2, &currentGeneration[i]);
		}

		//...with a little bit of fallout
		MutateCurrentPopulation();
		++generationCounter;
	}

	void GeneticLearning::ScoreGeneration()
	{
		//clear scores buffer
		memset(scores, 0, sizeof(int)*populationSize);


		TicTacToeGame::MoveResult result;

		for (size_t networkIndex = 0; networkIndex < populationSize; networkIndex++)
		{
			for (size_t matchNumber = 0; matchNumber < gamesPlayedPerScoring; matchNumber++)
			{
				const bool playsAsX = (matchNumber & 1) == 0;
				TicTacToeGame::IPlayer* player1 = playsAsX ? thisPlayer : opponentToTestAgainst;
				TicTacToeGame::IPlayer* player2 = playsAsX ? opponentToTestAgainst : thisPlayer;

				TicTacToeGame::TicTacToe game(player1, player2);

				result = TicTacToeGame::MoveResult::GAME_CONTINUES;
				while (result == TicTacToeGame::MoveResult::GAME_CONTINUES)
				{
					result = game.Update();
				}

				if (result == TicTacToeGame::MoveResult::O_WON || result == TicTacToeGame::MoveResult::X_WON)
				{
					const bool xWon = result == TicTacToeGame::MoveResult::X_WON;
					const bool networkWon = xWon == playsAsX;
					
					if (networkWon)
					{
						++scores[networkIndex];
					}
					else
					{
						--scores[networkIndex];
					}
				}
			}
		}
	}

	int GeneticLearning::GetCurrentGenerationCounter() const
	{
		return generationCounter;
	}

	const int* GeneticLearning::GetScores() const
	{
		return scores;
	}

	void GeneticLearning::CreateNetworks()
	{
		previousGeneration = new Neural::BasicNeuralNetwork[populationSize];
		for (size_t i = 0; i < populationSize; i++)
		{
			previousGeneration[i].InitializeLayers(networkLayerSizes, networkLayerCount);
		}

		currentGeneration = new Neural::BasicNeuralNetwork[populationSize];
		for (size_t i = 0; i < populationSize; i++)
		{
			currentGeneration[i].InitializeLayers(networkLayerSizes, networkLayerCount);
			Neural::HelperFunctions::RandomizeNeuralNetworkWeights(&currentGeneration[i], -1.0f, 1.0f);
			Neural::HelperFunctions::RandomizeNeuralNetworkTresholds(&currentGeneration[i], -1.0f, 1.0f);
		}
	}

	void GeneticLearning::MutateCurrentPopulation()
	{
		for (size_t networkIndex = 0; networkIndex < populationSize; networkIndex++)
		{
			Neural::BasicNeuralNetwork& thisNetwork = currentGeneration[networkIndex];

			for (size_t layerIndex = 0; layerIndex < networkLayerCount; layerIndex++)
			{
				Neural::Layer<float>& thisLayer = thisNetwork.layers[layerIndex];

				const size_t weightsCount = thisLayer.thisLayerSize * thisLayer.nextLayerSize;
				for (size_t weightIndex = 0; weightIndex < weightsCount; weightIndex++)
				{
					if (Neural::HelperFunctions::Random(1.0f) < mutationRate)
					{
						thisLayer.weights[weightIndex] = Neural::HelperFunctions::Random(2.0f) - 1.0f;
					}
				}

				const size_t tresholdsCount = thisLayer.thisLayerSize;
				for (size_t tresholdIndex = 0; tresholdIndex < tresholdsCount; tresholdIndex++)
				{
					if (Neural::HelperFunctions::Random(1.0f) < mutationRate)
					{
						thisLayer.tresholds[tresholdIndex] = Neural::HelperFunctions::Random(2.0f) - 1.0f;
					}
				}
			}
		}
	}

	void GeneticLearning::Crossover(const Neural::BasicNeuralNetwork & parent1, const Neural::BasicNeuralNetwork & parent2, Neural::BasicNeuralNetwork * child)
	{
		for (size_t layerIndex = 0; layerIndex < networkLayerCount - 1; ++layerIndex)
		{
			Neural::Layer<float>& thisLayer = child->layers[layerIndex];

			const size_t weightsCount = thisLayer.thisLayerSize * thisLayer.nextLayerSize;

			for (size_t weightIndex = 0; weightIndex < weightsCount; ++weightIndex)
			{
				const bool from1stParent = rand() & 1;
				thisLayer.weights[weightIndex] = (from1stParent ? parent1 : parent2).layers[layerIndex].weights[weightIndex];
			}

			const size_t tresholdsCount = thisLayer.thisLayerSize;

			for (size_t tresholdIndex = 0; tresholdIndex < tresholdsCount; ++tresholdIndex)
			{
				const bool from1stParent = rand() & 1;
				thisLayer.tresholds[tresholdIndex] = (from1stParent ? parent1 : parent2).layers[layerIndex].tresholds[tresholdIndex];
			}
		}
	}

	size_t GeneticLearning::FindFittestInCurrentGeneration(int*& scores)
	{
		int bestScore = INT_MIN;
		size_t bestIndividual = 0;

		for (size_t i = 0; i < populationSize; i++)
		{
			if (bestScore < scores[i])
			{
				bestScore = scores[i];
				bestIndividual = i;
			}
		}

		return bestIndividual;
	}

}
