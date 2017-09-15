#pragma once
#include <exception>
#include "Layer.h"


/************************************************************************/
/*                              DECLARATIONS                            */
/************************************************************************/

namespace Neural
{

	template<typename T>
	class NeuralNetwork
	{
	public:
		NeuralNetwork();
		NeuralNetwork(const size_t* layerSizes, const size_t layerCount);
		~NeuralNetwork();

		void InitializeLayers(const size_t* layerSizes, const size_t layerCount);
		const T* Compute(T* inputs, const size_t inputsSize, size_t& outputsSize);
	protected:
		void ClearInputs();
		inline void SetFirstLayerInputs(T* inputs);

	public:
		size_t layerCount;
		Layer<T>* layers;
	};

	using BasicNeuralNetwork = NeuralNetwork<float>;
	using DoublePrecisionNeuralNetwork = NeuralNetwork<double>;
}



/************************************************************************/
/*                              DEFINITIONS                             */
/************************************************************************/

namespace Neural
{
	template<typename T>
	NeuralNetwork<T>::NeuralNetwork()
	{
	}
	template<typename T>
	NeuralNetwork<T>::NeuralNetwork(const size_t * layerSizes, const size_t layerCount)
	{
		InitializeLayers(layerSizes, layerCount);
	}

	template<typename T>
	NeuralNetwork<T>::~NeuralNetwork()
	{
		delete[] layers;
	}

	template<typename T>
	void Neural::NeuralNetwork<T>::InitializeLayers(const size_t* layerSizes, const size_t layerCount)
	{
		if (layerCount <= 0)
		{
			throw std::exception("Invalid number of layers");
		}
		this->layerCount = layerCount;

		//Create buffer for layers and initialize their internal neuron buffers
		layers = new Layer<T>[layerCount];
		for (size_t i = 0; i < layerCount - 1; i++)
		{
			const size_t thisLayerSize = layerSizes[i];
			const size_t nextLayerSize = layerSizes[i + 1];
			layers[i].Initialize(thisLayerSize, nextLayerSize);
		}
		layers[layerCount - 1].Initialize(layerSizes[layerCount - 1], 0);
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
			Layer<T>& thisLayer = layers[layerIndex];
			Layer<T>& nextLayer = layers[layerIndex + 1];

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

		Layer<T>& lastLayer = layers[layerCount - 1];
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

