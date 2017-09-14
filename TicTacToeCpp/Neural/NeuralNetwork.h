#pragma once

namespace Neural
{

	template<typename T>
	class NeuralNetwork<T>
	{
	public:
		NeuralNetwork<T>();
		~NeuralNetwork<T>();
	};





	template<typename T>
	using BasicNeuralNetwork = NeuralNetwork<float>;
}


