#pragma once

namespace Neural
{
	template<typename T>
	class Layer<T>
	{
	public:
		Layer<T>(size_t thisLayerSize, size_t nextLayerSize);
		~Layer();

		void ClearInputs();
	public:
		T* tresholds;
		T* weights;
		T* inputs;

		size_t thisLayerSize;
		size_t nextLayerSize;
	};
}

