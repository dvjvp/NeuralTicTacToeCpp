// TicTacToeCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include "Neural/NeuralNetwork.h"
#include "Game/TicTacToe.h"
#include "Players/RandomPlayer.h"
#include "Players/NeuralNetworkPlayer.h"

using namespace TicTacToeGame;

int main()
{
	// For neural network helper functions (used by neural network player)
	// to correctly work, random stream must be first correctly seeded.
	srand((size_t)time(0));		

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

	getchar();
    return EXIT_SUCCESS;
}

