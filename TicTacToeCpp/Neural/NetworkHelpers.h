#pragma once
#include "NeuralNetwork.h"

namespace Neural
{
	namespace HelperFunctions
	{
		/**
		* Gets random number between 0 and @p max
		* Should probably be used with floating point types.
		* It probably won't work with integers (i guess it'd return 0).
		* @param max - maximum possible value to return [exclusive]
		* @return Some random value between 0 and @p max
		*/
		template<typename T>
		inline T Random(T max)
		{
			return ((T)rand() / (T)(RAND_MAX)) * max;
		}


		/**
		* @brief Randomizes threshold values of each neuron in NeuralNetwork
		* 
		* Sets value of each neuron.threshold in NeuralNetwork to some random value between @p min and @p max
		* @param network - network to randomize
		* @param min - minimal value possible [inclusive]
		* @param max - maximum value possible [exclusive]
		*/
		template<typename T>
		void RandomizeNeuralNetworkTresholds(NeuralNetwork<T>* network, const T min, const T max)
		{
			const T randomMax = max - min;

			for (size_t i = 0; i < network->layerCount; ++i)
			{
				Neural::Layer<T>& thisLayer = network->layers[i];

				//tresholds
				for (size_t j = 0; j < thisLayer.thisLayerSize; j++)
				{
					thisLayer.tresholds[j] = Random(randomMax) + min;
				}
			}
		}

		/**
		* @brief Randomizes weights values of each neuron in NeuralNetwork
		*
		* Sets value of each neuron.weight in NeuralNetwork to some random value between @p min and @p max
		* @param network - network to randomize
		* @param min - minimal value possible [inclusive]
		* @param max - maximum value possible [exclusive]
		*/
		template<typename T>
		void RandomizeNeuralNetworkWeights(NeuralNetwork<T>* network, const T min, const T max)
		{
			const T randomMax = max - min;

			for (size_t i = 0; i < network->layerCount; ++i)
			{
				Neural::Layer<T>& thisLayer = network->layers[i];

				//weights
				for (size_t j = 0; j < thisLayer.thisLayerSize*thisLayer.nextLayerSize; j++)
				{
					thisLayer.weights[j] = Random(randomMax) + min;
				}
			}
		}
	}
	
}
