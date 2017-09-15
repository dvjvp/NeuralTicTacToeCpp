#include "GeneticLearning.h"
#include "../Neural/NetworkHelpers.h"
#include "../Game/IPlayer.h"
#include "../Players/NeuralNetworkPlayer.h"
#include "../Players/RandomPlayer.h"
#include "../Game/TicTacToe.h"


namespace Genetic
{

	GeneticLearning::GeneticLearning(const size_t * networkLayerSizes, const size_t networkLayerCount)
	{
		this->networkLayerSizes = networkLayerSizes;
		this->networkLayerCount = networkLayerCount;
	}

	GeneticLearning::~GeneticLearning()
	{
		delete[] networks;
		delete[] scores;

		delete opponentToTestAgainst;
		delete thisPlayer;
	}

	void GeneticLearning::Initialize()
	{
		CreateNetworks();
		scores = new int[populationSize];

		thisPlayer = new TicTacToeGame::NeuralNetworkPlayer(&networks[0]);
		opponentToTestAgainst = new TicTacToeGame::RandomPlayer();
	}

	void GeneticLearning::NextGeneration()
	{
		ScoreGeneration();

		//TODO: Crossing and survival of the fittest

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
			for (size_t matchNumber = 0; matchNumber < GAMES_PLAYED_PER_SCORING; matchNumber++)
			{
				const bool playsAsX = matchNumber % 2;
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

	void GeneticLearning::CreateNetworks()
	{
		networks = new Neural::BasicNeuralNetwork[populationSize];
		for (size_t i = 0; i < populationSize; i++)
		{
			networks->InitializeLayers(networkLayerSizes, networkLayerCount);
			Neural::HelperFunctions::RandomizeNeuralNetworkWeights(&networks[i], -1.0f, 1.0f);
			Neural::HelperFunctions::RandomizeNeuralNetworkTresholds(&networks[i], -1.0f, 1.0f);
		}
	}

	void GeneticLearning::MutateCurrentPopulation()
	{
		for (size_t networkIndex = 0; networkIndex < populationSize; networkIndex++)
		{
			Neural::BasicNeuralNetwork& thisNetwork = networks[networkIndex];


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

}
