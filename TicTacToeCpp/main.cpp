#include "stdafx.h"
#include <iostream>
#include <time.h>
#include "Neural/NeuralNetwork.h"
#include "Game/TicTacToe.h"
#include "Players/RandomPlayer.h"
#include "Players/NeuralNetworkPlayer.h"
#include "Genetic/GeneticLearning.h"
#include "Utility/Algorithms.h"

//#define SINGLE_NEURAL_NETWORK_GAME
#define GENETIC_LEARNING

using namespace TicTacToeGame;

float mean(const std::vector<int>& numbers);

int main()
{
	// For neural network helper functions (used by neural network player)
	// to correctly work, random stream must be first correctly seeded.
	srand((size_t)time(0));		

#ifdef SINGLE_NEURAL_NETWORK_GAME


	IPlayer* player1 = new RandomPlayer();
	IPlayer* player2 = new NeuralNetworkPlayer();

	TicTacToe game(player1, player2);
	MoveResult result = MoveResult::GAME_CONTINUES;
	while (result == MoveResult::GAME_CONTINUES)
	{
		system("cls");
		result = game.Update();
		std::cout << game.board << std::endl;
		getchar();
	}



	delete player1;
	delete player2;

	std::cout << "Game finished. ";
	if (result == MoveResult::TIE)
	{
		std::cout << "Tie." << std::endl;
	}
	else
	{
		std::cout << ToChar((Field)result) << " won." << std::endl;
	}
#endif // SINGLE_NEURAL_NETWORK_GAME

#ifdef GENETIC_LEARNING

	size_t layerSizes[] = { 9,9,9 };

	Genetic::GeneticLearning evolution(layerSizes, 3);
	evolution.populationSize = 16;
	evolution.gamesPlayedPerScoring = 15;
	evolution.survivalRate = 0.50f;
	evolution.mutationRate = 0.015f;
	evolution.elitism = true;

	evolution.Initialize();

	do 
	{
		system("cls");

		evolution.NextGeneration();

		const int* scores = evolution.GetScores();
		std::vector<int> networkScores(scores, scores + evolution.populationSize);
		std::vector<size_t> sortedFromWorst = sort_indexes(networkScores);
		
		std::cout << "Generation: " << evolution.GetCurrentGenerationCounter() << std::endl;
		std::cout << "Score from "<<evolution.gamesPlayedPerScoring<<" games against random AI (win adds +1 to score, lose -1 and tie 0):" 
			<< std::endl << std::endl;
		for (int i = sortedFromWorst.size() - 1; i >= 0 ; --i)
		{
			std::cout << scores[sortedFromWorst[i]] << '\t';
		}
		std::cout << std::endl << std::endl << "Mean score: " << mean(networkScores);
		getchar();

	} while (evolution.GetCurrentGenerationCounter() < 40);
#endif // GENETIC_LEARNING


	getchar();
    return EXIT_SUCCESS;
}

float mean(const std::vector<int>& numbers)
{
	float result = 0.0f;
	for (int i : numbers)
	{
		result += (float)i;
	}
	return result / (float)numbers.size();
}
