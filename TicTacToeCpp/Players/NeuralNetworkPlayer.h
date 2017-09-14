#pragma once
#include "../Game/IPlayer.h"
#include "../Neural/NeuralNetwork.h"

namespace TicTacToeGame
{

	class NeuralNetworkPlayer : public IPlayer
	{
	public:
		NeuralNetworkPlayer();
		~NeuralNetworkPlayer();

		virtual char MakeMove(const Board& currentBoard) override;
	protected:
		float* GenerateInputs(const Board& currentBoard);
		char ProcessOutputs(const float* outputs);
		inline float FieldToFloat(const Field& field);
	private:
		Neural::BasicNeuralNetwork* network;
	};
}


