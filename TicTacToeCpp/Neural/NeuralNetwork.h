#pragma once
#include <exception>
#include "Layer.h"


/************************************************************************/
/*                              DECLARATIONS                            */
/************************************************************************/

namespace Neural
{
	/*
	* @brief Neural network
	*
	* 
	*/
	template<typename T>
	class NeuralNetwork
	{
	public:
		/**
		* @brief Empty constructor
		* 
		* Does nothing. Is used when instantiating an array of NeuralNetworks. Requires
		* @see InitializeLayers() to be called manually right after c-tor to work correctly
		*/
		NeuralNetwork();
		/**
		* @brief Constructor you can use for quick network creation
		*
		* Automatically calls InitializeLayers().
		*
		* @param layerSizes - number of neurons in each layer
		* @param layerCount - size of array @p layerSizes. Also number of total layers in this network
		*/
		NeuralNetwork(const size_t* layerSizes, const size_t layerCount);
		/**
		* @brief Destructor
		* 
		* Frees buffers initialized by InitializeLayers() method
		*/
		~NeuralNetwork();
		/**
		* @brief Copies data between two networks with the same layerCount and layerSizes
		* 
		* Performs memcpy() on each buffer of each layer from @p other to this network
		*
		* @param other - other NeuralNetwork to copy neurons' data from
		* @return this (fluent interface)
		*/
		NeuralNetwork& operator=(const NeuralNetwork& other);

		/**
		* @brief Second part of the constructor. Allocates buffer for layers and initializes them
		* 
		* Automatically called when using 2-parameters c-tor:
		* @see NeuralNetwork(const size_t* layerSizes, const size_t layerCount)
		* @param layerSizes - number of neurons in each layer
		* @param layerCount - size of array @p layerSizes. Also number of total layers in this network
		*/
		void InitializeLayers(const size_t* layerSizes, const size_t layerCount);
		/**
		* @brief Runs neural network computations and returns values from output layer
		* 
		* Value returned is the actual buffer for data stored in the last layer, so
		* don't try to delete it or free it or something, it needs to exist until the end
		* of the neural network lifespan and then it's destructor will delete it for you anyway.
		* That's why it's const too - so you don't mess with it too much.
		*
		* @param inputs - inputs for computation
		* @param inputsSize - size of the inputs array
		* @param outputsSize - (output parameter) size of the output. In case you forgot.
		* @return values after processing by neural network
		*/
		const T* Compute(const T* inputs, const size_t inputsSize, size_t& outputsSize);
	protected:
		/**
		* @brief Clears values stored in each neuron
		* 
		* Set's all inputs' buffers' values to 0.
		*/
		void ClearInputs();
		/**
		* @brief Copies data to first layer inputs, making it input for the whole network
		* 
		* Copies data to first layer input buffer
		* @param inputs - data to copy
		*/
		inline void SetFirstLayerInputs(const T* inputs);

	public:
		//Number of layers in this network
		size_t layerCount;
		//Array of layers of which this neural network is composed
		//This array size is @see layerCount
		Layer<T>* layers = nullptr;
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
	NeuralNetwork<T> & NeuralNetwork<T>::operator=(const NeuralNetwork<T>& other)
	{
		for (size_t i = 0; i < layerCount; ++i)
		{
			const size_t weightsBufferSize = layers[i].thisLayerSize * layers[i].nextLayerSize;
			memcpy(layers[i].weights, other.layers[i].weights, sizeof(T)*weightsBufferSize);

			const size_t tresholdsBufferSize = layers[i].thisLayerSize;
			memcpy(layers[i].tresholds, other.layers[i].tresholds, sizeof(T)*tresholdsBufferSize);
		}

		return *this;
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
	const T* NeuralNetwork<T>::Compute(const T * inputs, const size_t inputsSize, size_t & outputsSize)
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
	inline void NeuralNetwork<T>::SetFirstLayerInputs(const T * inputs)
	{
		memcpy(layers[0].inputs, inputs, layers[0].thisLayerSize);
	}
}

