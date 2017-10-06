#pragma once
#include <fstream>
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
	
	namespace Files
	{
		template<typename T>
		NeuralNetwork<T>* LoadNetworkFromStream(std::istream& stream)
		{
			// Load network size
			size_t layersCount;
			stream >> layersCount;
			size_t* layersSizes = new size_t[layersCount];
			for (size_t i = 0; i < layersCount; i++)
			{
				stream >> layersSizes[i];
			}

			//Create network
			NeuralNetwork<T>* network = new NeuralNetwork<T>(layersSizes, layersCount);

			//Load neuron data
			for (size_t i = 0; i < layersCount; i++)
			{
				Layer<T>& thisLayer = network->layers[i];
				for (size_t j = 0; j < thisLayer.thisLayerSize; j++)
				{
					stream >> thisLayer.tresholds[j];
				}
				for (size_t j = 0; j < thisLayer.thisLayerSize*thisLayer.nextLayerSize; j++)
				{
					stream >> thisLayer.weights[j];
				}
			}

			delete layersSizes;
			return network;
		}

		template<typename T>
		NeuralNetwork<T>* LoadNetworkFromFile(const char* filepath)
		{
			std::ifstream file(filepath, std::ios::in
#ifdef NETWORKS_SAVED_AS_BINARY
				| std::ios::binary
#endif // NETWORKS_SAVED_AS_BINARY
			);

			return LoadNetworkFromStream<T>(file);
		}

		

		template<typename T>
		void SaveNetworkToFile(NeuralNetwork<T>* network, const char* filepath)
		{
			/* Create/Open file */

			std::ofstream file(filepath, std::ios::out | std::ios::trunc
#ifdef NETWORKS_SAVED_AS_BINARY
				| std::ios::binary
#endif // NETWORKS_SAVED_AS_BINARY
			);
			

			/* Definitions according to save mode */

#ifdef NETWORKS_SAVED_AS_BINARY
#define NEW_LINE
#define SPACE
#else
#define NEW_LINE file << std::endl;
#define SPACE file << ' ';
#endif // NETWORKS_SAVED_AS_BINARY


			/* Save network sizes */

			file << network->layerCount;
			NEW_LINE
			for (size_t i = 0; i < network->layerCount - 1; i++)
			{
				file << network->layers[i].thisLayerSize;
				SPACE
			}
			file << network->layers[network->layerCount - 1].thisLayerSize;
			NEW_LINE
			NEW_LINE

			/* Save network data */

			for (size_t i = 0; i < network->layerCount; i++)
			{
				Layer<T>& thisLayer = network->layers[i];
				for (size_t j = 0; j < thisLayer.thisLayerSize; j++)
				{
					file << thisLayer.tresholds[j];
					SPACE
				}
				NEW_LINE
				for (size_t j = 0; j < thisLayer.thisLayerSize*thisLayer.nextLayerSize; j++)
				{
					file << thisLayer.weights[j];
					SPACE
				}

				NEW_LINE
				NEW_LINE
			}


			/* Undefine stuff */

#undef NEW_LINE
#undef SPACE

		}
	}
}
