#pragma once
#include "../Game/IPlayer.h"
#include "../Neural/NeuralNetwork.h"

namespace TicTacToeGame
{

	class NeuralNetworkPlayer : public IPlayer
	{
	public:
		NeuralNetworkPlayer();
		NeuralNetworkPlayer(Neural::BasicNeuralNetwork* network);
		~NeuralNetworkPlayer();

		virtual char MakeMove(const Board& currentBoard) override;
		void SetNewNeuralNetwork(Neural::BasicNeuralNetwork* network);
	protected:
		float* GenerateInputs(const Board& currentBoard);
		char ProcessOutputs(const float* outputs);
		inline float FieldToFloat(const Field& field);
		inline float Random(const float _max);
	private:
		Neural::BasicNeuralNetwork* network;
	};
}


