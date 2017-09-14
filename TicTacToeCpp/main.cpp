// TicTacToeCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Neural/Layer.cpp"
#include "Neural/NeuralNetwork.cpp"


int main()
{
	size_t layerSizes[] = { 4,3,1 };

	Neural::BasicNeuralNetwork network(layerSizes, 3);


	getchar();
    return 0;
}

