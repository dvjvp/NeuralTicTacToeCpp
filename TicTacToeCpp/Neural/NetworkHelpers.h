#pragma once
#include "NeuralNetwork.h"

namespace Neural
{
	namespace HelperFunctions
	{
		template<typename T>
		inline T Random(T max)
		{
			return ((T)rand() / (T)(RAND_MAX)) * max;
		}

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
