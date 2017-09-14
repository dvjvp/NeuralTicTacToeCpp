#include "Layer.h"

namespace Neural
{

	template<typename T>
	Layer<T>::Layer(size_t thisLayerSize, size_t nextLayerSize)
	{
		this->thisLayerSize = thisLayerSize;
		this->nextLayerSize = nextLayerSize;

		tresholds = malloc(thisLayerSize * sizeof(T));
		weights = malloc(thisLayerSize*nextLayerSize * sizeof(T));
		inputs = malloc(thisLayerSize * sizeof(T));
	}

	template<typename T>
	Layer<T>::~Layer()
	{
		free(tresholds);
		free(weights);
		free(inputs);
	}
	template<typename T>
	void Layer<T>::ClearInputs()
	{
		memset(inputs, (T)0, thisLayerSize * sizeof(T));
	}
}
