#pragma once
#include "../Game/IPlayer.h"
#include "../Neural/NeuralNetwork.h"

namespace TicTacToeGame
{
	/*
	* Player for TicTacToe that uses Neural Network to calculate his moves
	*/
	class NeuralNetworkPlayer : public IPlayer
	{
	public:
		NeuralNetworkPlayer();
		NeuralNetworkPlayer(Neural::BasicNeuralNetwork* network);
		~NeuralNetworkPlayer();

		virtual char MakeMove(const Board& currentBoard) override;
		Neural::BasicNeuralNetwork* SetNewNeuralNetwork(Neural::BasicNeuralNetwork* network);
	protected:
		float* GenerateInputs(const Board& currentBoard);
		char ProcessOutputs(const float* outputs);
		inline float FieldToFloat(const Field& field);
	private:
		Neural::BasicNeuralNetwork* network;
		bool dontDeleteNetworkOnDestructor = false;
	};
}


