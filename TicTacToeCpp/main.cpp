// TicTacToeCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Neural/NeuralNetwork.h"


int main()
{
	size_t layerSizes[] = { 4,3,1 };

	Neural::BasicNeuralNetwork network(layerSizes, 3);


	float sampleInputs[] = { 1.0f,1.0f,1.0f,1.0f };
	size_t outputSize;
	const float* output = network.Compute(sampleInputs, 4, outputSize);

	for (size_t i = 0; i < outputSize; i++)
	{
		std::cout << output[i];
	}


	getchar();
    return 0;
}

