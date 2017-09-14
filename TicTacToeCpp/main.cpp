// TicTacToeCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Neural/NeuralNetwork.h"
#include "Game/TicTacToe.h"
#include "Players/RandomPlayer.h"
#include "Players/NeuralNetworkPlayer.h"

using namespace TicTacToeGame;

int main()
{
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

	//float sampleInputs[] = { 1.0f,1.0f,1.0f,1.0f };
	//size_t outputSize;
	//for (size_t i = 0; i < network.layerCount; ++i)
	//{
	//	Neural::Layer<float>& thisLayer = network.layers[i];
	//	for (size_t j = 0; j < thisLayer.thisLayerSize; j++)
	//	{
	//		thisLayer.tresholds[j] = 0.0f;
	//	}
	//	for (size_t j = 0; j < thisLayer.thisLayerSize*thisLayer.nextLayerSize; j++)
	//	{
	//		thisLayer.weights[j] = 1.0f;
	//	}
	//}
	//const float* output = network.Compute(sampleInputs, 4, outputSize);
	//for (size_t i = 0; i < outputSize; i++)
	//{
	//	std::cout << output[i];
	//}


	getchar();
    return EXIT_SUCCESS;
}

