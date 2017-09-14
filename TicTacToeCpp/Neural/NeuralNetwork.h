#pragma once
#include "Layer.h"

namespace Neural
{

	template<typename T>
	class NeuralNetwork
	{
	public:
		NeuralNetwork(size_t* layerSizes, size_t layerCount);
		~NeuralNetwork();

		const T* Compute(T* inputs, const size_t inputsSize, size_t& outputsSize);
	protected:
		void ClearInputs();
		inline void SetFirstLayerInputs(T* inputs);

	public:
		size_t layerCount;
		Layer<T>* layers;
	};





	template<typename T>
	using BasicNeuralNetwork = NeuralNetwork<float>;
}


