#include "NeuralNetworkPlayer.h"
#include "../Utility/Algorithms.h"
#include "../Neural/NetworkHelpers.h"

namespace TicTacToeGame
{

	NeuralNetworkPlayer::NeuralNetworkPlayer()
	{
		size_t layerSizes[] = { 9,9,9 };

		network = new Neural::BasicNeuralNetwork(layerSizes, 3);

		//randomize weights and tresholds
		Neural::HelperFunctions::RandomizeNeuralNetworkTresholds(network, 0.0f, 0.35f);
		Neural::HelperFunctions::RandomizeNeuralNetworkWeights(network, 0.0f, 1.0f);
	}

	NeuralNetworkPlayer::NeuralNetworkPlayer(Neural::BasicNeuralNetwork * network)
	{
		this->network = network;
		dontDeleteNetworkOnDestructor = true;
	}


	NeuralNetworkPlayer::~NeuralNetworkPlayer()
	{
		if (!dontDeleteNetworkOnDestructor)
		{
			delete network;
		}
	}

	char NeuralNetworkPlayer::MakeMove(const Board& currentBoard)
	{
		float* inputs = GenerateInputs(currentBoard);
		size_t unusedTemp;
		const float* outputs = network->Compute(inputs, Board::BOARD_SIZE, unusedTemp);
		return ProcessOutputs(outputs);
	}

	void NeuralNetworkPlayer::SetNewNeuralNetwork(Neural::BasicNeuralNetwork * network)
	{
		delete this->network;
		this->network = network;
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

}
