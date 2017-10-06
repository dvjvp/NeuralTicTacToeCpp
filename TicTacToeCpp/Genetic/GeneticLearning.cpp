#include <iostream>
#include "GeneticLearning.h"
#include "../Game/TicTacToe.h"
#include "../Neural/NetworkHelpers.h"
#include "../Players/NeuralNetworkPlayer.h"
#include "../Utility/Algorithms.h"

//#define DRAW_BOARDS

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

		//delete opponentToTestAgainst;
		delete thisPlayer;
	}

	void GeneticLearning::Initialize(TicTacToeGame::IPlayer* opponentToTestAgainst)
	{
		if (populationSize <= 3)
		{
			throw std::exception("Population size invalid. Algorithm can't work with that.");
		}

		CreateNetworks();
		scores = new int[populationSize];

		thisPlayer = new TicTacToeGame::NeuralNetworkPlayer(&currentGeneration[0]);
		this->opponentToTestAgainst = opponentToTestAgainst;
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


		std::vector<int> networkScores(scores, scores + populationSize);
		std::vector<size_t> sortedFromWorst = sort_indexes(networkScores);

		const size_t individualsThatGetToProcreate = (size_t)(survivalRate * populationSize);
#define RANDOM_SURVIVOR_INDEX (sortedFromWorst[(rand() % individualsThatGetToProcreate) + populationSize - individualsThatGetToProcreate])

		if (elitism)
		{
			//save best individual
			currentGeneration[0] = previousGeneration[sortedFromWorst.back()];
		}
		const size_t startIndex = elitism ? 1 : 0;

		//procreation for science
		for (size_t i = startIndex; i < populationSize; i++)
		{
			const size_t index1 = RANDOM_SURVIVOR_INDEX;
			const int scoreIndex1 = scores[index1];
			Neural::BasicNeuralNetwork& parent1 = previousGeneration[index1];
			const size_t index2 = RANDOM_SURVIVOR_INDEX;
			const int scoreIndex2 = scores[index2];
			Neural::BasicNeuralNetwork& parent2 = previousGeneration[index2];

			Crossover(parent1, parent2, &currentGeneration[i]);
		}
#undef RANDOM_SURVIVOR_INDEX
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
				thisPlayer->SetNewNeuralNetwork(&currentGeneration[networkIndex]);
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
#ifdef DRAW_BOARDS
					std::cout << game.board << std::endl;
#endif // DRAW_BOARDS

				}
			}
#ifdef DRAW_BOARDS
			std::cout << "--------------------" << std::endl;
#endif // DRAW_BOARDS

		}
	}

	size_t GeneticLearning::GetCurrentGenerationCounter() const
	{
		return generationCounter;
	}

	const int* GeneticLearning::GetScores() const
	{
		return scores;
	}

	Neural::BasicNeuralNetwork& GeneticLearning::GetBest()
	{
		return currentGeneration[FindFittestInCurrentGeneration(scores)];
	}

	void GeneticLearning::CreateNetworks()
	{
		previousGeneration = new Neural::BasicNeuralNetwork[populationSize];
		for (size_t i = 0; i < populationSize; i++)
		{
			previousGeneration[i].InitializeLayers(networkLayerSizes, networkLayerCount);
			Neural::HelperFunctions::RandomizeNeuralNetworkWeights(&previousGeneration[i], -1.0f, 1.0f);
			Neural::HelperFunctions::RandomizeNeuralNetworkTresholds(&previousGeneration[i], -1.0f, 1.0f);
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
