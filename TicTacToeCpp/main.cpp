#include "stdafx.h"
#include <iostream>
#include <time.h>
#include "Game/TicTacToe.h"
#include "Genetic/GeneticLearning.h"
#include "Neural/NetworkHelpers.h"
#include "Neural/NeuralNetwork.h"
#include "Players/ContinuusPlayer.h"
#include "Players/HumanPlayer.h"
#include "Players/NeuralNetworkPlayer.h"
#include "Players/RandomPlayer.h"
#include "Utility/Algorithms.h"


using namespace TicTacToeGame;

/**
 @brief Calculates arithmetic mean (average value) of passed numbers
 * 
 Adds all the values together, then divides it by their amount to get the average
 @param numbers - values for which to find the average
 @return Average value (Arithmetic mean of said numbers)
 */
float mean(const std::vector<int>& numbers);
/**
 @brief Prints scores to standard output (std::cout)
 * 
 Sorts the values, then prints them sorted for clarity
 @param scores - values to print
 */
void printScores(std::vector<int>& scores);

#define PRINT_MESSAGE(MESSAGE) std::cout << MESSAGE << std::endl; getchar()

/**
 @brief Entry point of application
 * 
 Shows usage of GeneticLearning of NeuralNetworks, allows to quickly modify it's parameters
 and is well-documented. It's a show-off point of the whole program basically
 @return 0 if exited correctly, that is if everything went smoothly. Returns some cryptic non-0 value
 and crashes instead when something goes wrong. That shouldn't happen' though. I think.
 */
int main()
{
	// For many functions here to correctly work, random stream must be first correctly seeded.
	srand((size_t)time(0));		



	size_t layerSizes[] = { 9,9,9,9,9 };	//number of neurons in each consecutive layer of our neural nets

	Genetic::GeneticLearning evolution(layerSizes, 5);
	evolution.populationSize = 20;			//in every generation, 20 networks will compete to be the best
	evolution.gamesPlayedPerScoring = 20;	//their score is determined from playing 20 games against some other AI (defined below)
	evolution.survivalRate = 0.20f;			//only about 20% of networks will get to procreate and have their genes passed on
	evolution.mutationRate = 0.015f;		//to better adapt to environment, each generation will have 1.5% of their genes randomized
	evolution.elitism = true;				//the best individual from previous population gets to survive to the next one


	IPlayer* opponentToTrainAgainst = new RandomPlayer();	//opponent our NeuralNetworks will train to beat
	evolution.Initialize(opponentToTrainAgainst);			//initialize buffers. Above values SHOULD NOT be changed after this point

	do 
	{
		evolution.NextGeneration();		//score current generation, then let them procreate and evolve

		const int* scores = evolution.GetScores();	//receive scores from PREVIOUS generation
		std::vector<int> networkScores(scores, scores + evolution.populationSize);	//sorting is performed on vector for convenience
		
		//display current data
		std::cout << "Generation: " << evolution.GetCurrentGenerationCounter() << std::endl;
		std::cout << "Score from " << evolution.gamesPlayedPerScoring <<
			" games against random AI (win adds +5 to score, lose -10 and tie +1):" << std::endl << std::endl;
		
		printScores(networkScores);

		//getchar();
		system("cls");

	} while (evolution.GetCurrentGenerationCounter() < 40);

	std::cout << "Final scores (generation " << evolution.GetCurrentGenerationCounter() << "):" << std::endl;
	{
		evolution.ScoreGeneration();	//force scoring current generation. Usually it happens automatically
										//when NextGeneration() gets called
		const int* scores = evolution.GetScores();
		std::vector<int> networkScores(scores, scores + evolution.populationSize);
		printScores(networkScores);
	}



	// Now let's play against our trained AI!
	// But let's save first, so we can restore it in the future without further training

	PRINT_MESSAGE("Program finished. Trying to save...");
	const char* filepath = "myBestNeuralNetwork.nn";

	Neural::BasicNeuralNetwork& network0 = evolution.GetBest();
	Neural::Files::SaveNetworkToFile(&network0, filepath);

	PRINT_MESSAGE("Success! Trying to load...");
	Neural::BasicNeuralNetwork* network1 = Neural::Files::LoadNetworkFromFile<float>(filepath);

	PRINT_MESSAGE("Success! Starting game...");
	TicTacToeGame::NeuralNetworkPlayer networkPlayer(network1);
	TicTacToeGame::HumanPlayer humanPlayer;
	TicTacToeGame::MoveResult result = TicTacToeGame::MoveResult::GAME_CONTINUES;

	{
		TicTacToeGame::TicTacToe game(&networkPlayer, &humanPlayer);

		while (result == TicTacToeGame::MoveResult::GAME_CONTINUES)
		{
			result = game.Update();
		}
		switch (result)
		{
		case TicTacToeGame::MoveResult::X_WON:
			std::cout << ("You lost to AI, when it was going first");
			break;
		case TicTacToeGame::MoveResult::O_WON:
			std::cout << ("You won with AI, when it was going first");
			break;
		case TicTacToeGame::MoveResult::TIE:
			std::cout << ("You tied with AI, when it was going first");
			break;
		}

		std::cout << std::endl << game.board << std::endl;
	}
	
	PRINT_MESSAGE("Preparing second game...");
	getchar();
	system("cls");
	result = TicTacToeGame::MoveResult::GAME_CONTINUES;
	{
		TicTacToeGame::TicTacToe game(&humanPlayer, &networkPlayer);
		while (result == TicTacToeGame::MoveResult::GAME_CONTINUES)
		{
			result = game.Update();
		}
		switch (result)
		{
		case TicTacToeGame::MoveResult::X_WON:
			std::cout << ("You won with AI, when going first");
			break;
		case TicTacToeGame::MoveResult::O_WON:
			std::cout << ("You lost to AI, when going first");
			break;
		case TicTacToeGame::MoveResult::TIE:
			std::cout << ("You tied with AI, when going first");
			break;
		}

		std::cout << std::endl << game.board << std::endl;
	}

	PRINT_MESSAGE("End of the simulation.");
	getchar();

	delete opponentToTrainAgainst;
	delete network1;
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

void printScores(std::vector<int>& scores)
{
	std::vector<size_t> sortedFromWorst = sort_indexes(scores);	//sort them, so they'll look cleaner when displayed

	for (int i = sortedFromWorst.size() - 1; i >= 0; --i)
	{
		std::cout << scores[sortedFromWorst[i]] << '\t';
	}

	std::cout << std::endl << std::endl << "Mean score: " << mean(scores) << std::endl;
}


