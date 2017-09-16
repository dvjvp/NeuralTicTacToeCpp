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
		/**
		* @brief Creates new neural network and sets it's values to random and uses it to play
		* 
		* Network will be automatically deleted when destructor hits if player created using this c-tor
		*/
		NeuralNetworkPlayer();
		/**
		* @brief Takes a pre-created neural network and uses it to play
		*
		* Network will be not be automatically deleted by destructor if player created using this c-tor
		*/
		NeuralNetworkPlayer(Neural::BasicNeuralNetwork* network);
		/**
		* @brief Destructor for NeuralNetworkPlayer
		* 
		* Depending on the method of player creation, this destructor will or will not perform delete 
		* on network that was used by this player
		*/
		~NeuralNetworkPlayer();

		//From TicTacToeGame::IPlayer interface
		virtual char MakeMove(const Board& currentBoard) override;

		/**
		* @brief Sets new neural network for this player to use
		* 
		* @param network - new neural network to use
		* @return previous neural network (you should handle deleting it yourself)
		*/
		Neural::BasicNeuralNetwork* SetNewNeuralNetwork(Neural::BasicNeuralNetwork* network);
	protected:
		/**
		* @brief Transforms current board state into neural network inputs
		* 
		* @param currentBoard - current board state
		* @return (float[]) array of inputs for neural network
		*/
		float* GenerateInputs(const Board& currentBoard);
		/**
		* @brief Transforms output from neural network into a move
		* 
		* Sorts outputs array and takes first of them that is a valid move
		* @param outputs - outputs from neural network
		* @return char - move
		*/
		char ProcessOutputs(const float* outputs);
		/**
		* @brief Transforms a single board piece into a single piece of neural network input
		* 
		* Allows the same network to play as X-s and O-s - it hides symbols behind line of abstraction
		* "Your symbol", "nothing" and "opponent's symbol"
		* @param field a single board piece: (Empty/X/O)
		* @return float representation of said field that is invariant to what color (X/O) is network playing as
		*/
		inline float FieldToFloat(const Field& field);
	private:
		//Neural network used by this player
		Neural::BasicNeuralNetwork* network;
		//According to c-tor chosen this variable controls whether to destroy internal neural network in d-tor
		bool dontDeleteNetworkOnDestructor = false;
	};
}


