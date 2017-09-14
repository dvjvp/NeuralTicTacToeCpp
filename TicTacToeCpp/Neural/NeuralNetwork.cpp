#include "NeuralNetwork.h"
#include "Layer.h"

namespace Neural
{
	template<typename T>
	NeuralNetwork<T>::NeuralNetwork(size_t * layerSizes, size_t layerCount)
	{
		if (layerCount <= 0)
		{
			throw std::exception("Invalid number of layers");
		}

		//Create buffer for layers and initialize their internal neuron buffers
		layers = malloc(layerCount * sizeof(Layer<T>));
		for (size_t i = 0; i < layerCount - 1; i++)
		{
			const size_t thisLayerSize = layerSizes[i];
			const size_t nextLayerSize = layerSizes[i + 1];
			layers[i] = Layer<T>(thisLayerSize, nextLayerSize);
		}
		layers[layerCount - 1] = Layer<T>(layerSizes[layerCount - 1], 0);

	}

	template<typename T>
	NeuralNetwork<T>::~NeuralNetwork()
	{
		for (size_t i = 0; i < layerCount; i++)
		{
			layers[i].~Layer();
		}

		free(layers);
	}


	template<typename T>
	const T* NeuralNetwork<T>::Compute(T * inputs, const size_t inputsSize, size_t & outputsSize)
	{
		//assertion
		if (inputsSize != layers[0].thisLayerSize)
		{
			throw std::exception("Invalid input size for neural network computation");
		}

		//Clear previous inputs & Set new inputs
		ClearInputs();
		SetFirstLayerInputs(inputs);

		for (size_t layerIndex = 0; layerIndex < layerCount - 1; layerIndex++)
		{
			Layer& thisLayer = layers[layerIndex];
			Layer& nextLayer = layers[layerIndex + 1];

			for (size_t nodeIndex = 0; nodeIndex < thisLayer.thisLayerSize; nodeIndex++)
			{
				for (size_t targetIndex = 0; targetIndex < thisLayer.nextLayerSize; targetIndex++)
				{
					T temp = thisLayer.inputs[nodeIndex] * thisLayer.GetWeight(nodeIndex, targetIndex);
					//TODO: Activation function here

					if (temp >= thisLayer.tresholds[nodeIndex])
					{
						nextLayer.inputs[targetIndex] += temp;
					}
				}
			}
		}

		Layer& lastLayer = layers[layerCount - 1];
		outputsSize = lastLayer.thisLayerSize;
		return lastLayer.inputs;
	}


	template<typename T>
	void NeuralNetwork<T>::ClearInputs()
	{
		for (size_t i = 0; i < layerCount; i++)
		{
			layers[i].ClearInputs();
		}
	}
	template<typename T>
	inline void NeuralNetwork<T>::SetFirstLayerInputs(T * inputs)
	{
		for (size_t i = 0; i < layers[0].thisLayerSize; i++)
		{
			layers[0].inputs[i] = inputs[i];
		}
	}
}

