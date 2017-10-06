#pragma once
#include "../Neural/NeuralNetwork.h"

namespace TicTacToeGame
{
	class IPlayer;				//just some predefinition, so i don't have to include header here. 
								//Less headers included in .h files, the better, right?
	class NeuralNetworkPlayer;	//same thing as above
}

namespace Genetic
{
	/**
	* @brief Class for teaching neural networks stuff
	*
	* Class handling process of teaching neural networks via trial-and-error method, that is evolution
	*/
	class GeneticLearning
	{
	public:
		/**
		* @brief Creates new instance of learning process using evolution
		* 
		* Prepares some data before initialization. Layer sizes to be exact 
		* @param networkLayerSizes - array specifying how many neurons should be in each layer
		* @param networkLayerCount - number of elements in @p networkLayerSizes array
		*/
		GeneticLearning(const size_t* networkLayerSizes, const size_t networkLayerCount);
		/**
		* @brief Destructor for GeneticLearning class
		* 
		* frees all buffers used by the learning process: all neural networks (and their layers),
		* buffer for storing score data and player object used by learning process to transfer
		* neural network outputs to moves on the board
		*/
		~GeneticLearning();

		/**
		* @brief Initializes all data needed for learning process to run. This method <b>HAS TO</b>
		* be run before NextGeneration(), ScoreGeneration() or GetScores() will be called.
		* 
		* After this method is called, learning parameters (gamesPlayedPerScoring, populationSize,
		* mutationRate, survivalRate, elitism) should NOT be changed. This method should be run once and only once.
		* After initial setting of learning parameters and before it starts running. It's basically second part of
		* the constructor.
		* 
		* @param opponentToTestAgainst - Neural networks will train to be as good as possible against this one opponent
		*/
		void Initialize(TicTacToeGame::IPlayer* opponentToTestAgainst);
		/**
		* @brief Moves learning process one generation forward
		* 
		* Calculates scores for current generation, handles their reproduction and mutation processes.
		* Use this method to push learning process forward
		*/
		void NextGeneration();
		/**
		* @brief Scores current generation. Scores then are saved in a buffer that can be obtained using
		* @see GetCurrentGenerationCounter() method.
		* 
		* Plays (gamesPlayedPerScoring) games with each neural network from current generation against
		* (opponentToTestAgainst) and save the scores in internal score buffer.
		* This method is called automatically at the beginning of 
		* @see NextGeneration() function, so there's no need to call it manually. 
		* It can still be called to get current scores after learning process stops.
		*/
		void ScoreGeneration();

		/**
		* @brief Gets number of current generation
		* 
		* Shows for how long networks have been learning
		* 
		* @return (unsigned int) current generation number
		*/
		size_t GetCurrentGenerationCounter() const;
		/**
		* @brief Returns array of scores from last ScoreGeneration().
		* Is the address of the actual buffer, that's why it's const.
		* The arrays has (populationSize) elements.
		* @return Scores from last ScoreGeneration() (most likely data for previous generation)
		*/
		const int* GetScores() const;

		Neural::BasicNeuralNetwork& GetBest();
	protected:
		void CreateNetworks();

		/**
		* @brief Mutates current generation
		* 
		* Randomly changes some values in neural network genome, which may
		* result in better or worse performance. This function allows specimens to
		* adapt to their environment. It's the "trial" of the trail-and-error learning process
		* and whether it was error or not is later checked in
		* @see ScoreGeneration()
		*/
		void MutateCurrentPopulation();
		/**
		* @brief Changes genome of @p child as if it was a child of @p parent1 and @p parent2
		* 
		* Copies genes randomly from @p parent1 or @p parent to @p child's genome.
		* @param parent1 - first individual to copy genes from
		* @param parent2 - second individual to copy genes from
		* @param child - (already created) neural network to copy the genes to
		*/
		void Crossover(const Neural::BasicNeuralNetwork& parent1, const Neural::BasicNeuralNetwork& parent2, Neural::BasicNeuralNetwork* child);
	private:
		/**
		* @brief Finds the best individual in a generation
		* 
		* Returns index of best individual from ones scored with given score parameter
		* @param scores - score values to find the best from
		* @return index of biggest value from scores
		*/
		size_t FindFittestInCurrentGeneration(int*& scores);


	public:
		//Opponent our NeuralNetworks will train to beat
		class TicTacToeGame::IPlayer* opponentToTestAgainst;

		//How many games should be played against (opponentToTestAgains) during ScoreGeneration()
		size_t gamesPlayedPerScoring = 10;
		//How many networks should exist in one population?
		size_t populationSize = 16;
		//0-1 value showing how many genes should be randomized each generation
		float mutationRate = 0.015f;
		//0-1 value showing how many networks should be allowed to reproduce (only the best one get to reproduce)
		float survivalRate = 0.50f;
		//whether the best network from previous generation should live on to the next one
		bool elitism = true;

	protected:
		//Array specifying how many neurons should be in each layer
		const size_t* networkLayerSizes;
		//How many layers are there in a neural network
		size_t networkLayerCount;
		
		//For how long are we teaching neural networks?
		size_t generationCounter = 0;

		//IPlayer interface using which neural networks can be connected to a game of TicTacToe
		class TicTacToeGame::NeuralNetworkPlayer* thisPlayer;
		//Newest generation of our neural networks
		Neural::BasicNeuralNetwork* currentGeneration;
		//Storing buffer with previous generation avoid constantly allocating memory, which is SLOOOOOOOW
		Neural::BasicNeuralNetwork* previousGeneration;
		
		//Buffer for scores from games played by neural networks
		int* scores;
	};

}

