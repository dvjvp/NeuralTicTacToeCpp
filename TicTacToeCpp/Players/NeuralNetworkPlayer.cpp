#include "NeuralNetworkPlayer.h"
#include "../Utility/Algorithms.h"
#include <time.h>
#include <cstdlib>

namespace TicTacToeGame
{

	NeuralNetworkPlayer::NeuralNetworkPlayer()
	{
		size_t layerSizes[] = { 9,9,9 };

		network = new Neural::BasicNeuralNetwork(layerSizes, 3);

		//randomize weights and tresholds
		srand((size_t)time(0));
		for (size_t i = 0; i < network->layerCount; ++i)
		{
			Neural::Layer<float>& thisLayer = network->layers[i];

			//tresholds
			for (size_t j = 0; j < thisLayer.thisLayerSize; j++)
			{
				thisLayer.tresholds[j] = Random(0.35f);
			}
			//weights
			for (size_t j = 0; j < thisLayer.thisLayerSize*thisLayer.nextLayerSize; j++)
			{
				thisLayer.weights[j] = Random(1.0f);
			}
		}
	}


	NeuralNetworkPlayer::~NeuralNetworkPlayer()
	{
		delete network;
	}

	char NeuralNetworkPlayer::MakeMove(const Board& currentBoard)
	{
		float* inputs = GenerateInputs(currentBoard);
		size_t unusedTemp;
		const float* outputs = network->Compute(inputs, Board::BOARD_SIZE, unusedTemp);
		return ProcessOutputs(outputs);
	}

	float* NeuralNetworkPlayer::GenerateInputs(const Board& currentBoard)
	{
		float* input = new float[Board::BOARD_SIZE];
		for (char i = 0; i < Board::BOARD_SIZE; i++)
		{
			input[i] = FieldToFloat(currentBoard.GetSquare(i));
		}
		return input;
	}

	char NeuralNetworkPlayer::ProcessOutputs(const float* outputs)
	{
		std::vector<float> _outputs( outputs, outputs + Board::BOARD_SIZE);
		std::vector<size_t>& sortedIndexes = sort_indexes(_outputs);
		for (size_t i = 0; i < Board::BOARD_SIZE; i++)
		{
			if (IsMoveValid((char)sortedIndexes[i]))
			{
				return (char)sortedIndexes[i];
			}
		}
		return 0;
	}

	inline float NeuralNetworkPlayer::FieldToFloat(const Field& field)
	{
		if (field == Field::EMPTY) return 0.0f;
		if (field == GetColor()) return 1.0f;
		else return -1.0f;
	}

	inline float NeuralNetworkPlayer::Random(const float _max)
	{
		return ((float)rand() / (float)(RAND_MAX)) * _max;
	}

}
