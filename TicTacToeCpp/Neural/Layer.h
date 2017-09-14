#pragma once

/************************************************************************/
/*                              DECLARATIONS                            */
/************************************************************************/

namespace Neural
{
	template<typename T>
	class Layer
	{
	public:
		Layer();
		void Initialize(const size_t thisLayerSize, const size_t nextLayerSize);
		~Layer();

		void ClearInputs();

		inline T GetWeight(const size_t nodeInThisLayerIndex, const size_t nodeInNextLayerIndex) const;
	public:
		T* tresholds;
		T* weights;
		T* inputs;

		size_t thisLayerSize;
		size_t nextLayerSize;
	};
}




/************************************************************************/
/*                              DEFINITIONS                             */
/************************************************************************/

namespace Neural
{

	template<typename T>
	void Layer<T>::Initialize(const size_t thisLayerSize, const size_t nextLayerSize)
	{
		this->thisLayerSize = thisLayerSize;
		this->nextLayerSize = nextLayerSize;

		tresholds = (T*)malloc(thisLayerSize * sizeof(T));
		weights = (T*)malloc(thisLayerSize*nextLayerSize * sizeof(T));
		inputs = (T*)malloc(thisLayerSize * sizeof(T));
	}

	template<typename T>
	Layer<T>::Layer()
	{
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
	template<typename T>
	inline T Layer<T>::GetWeight(const size_t nodeInThisLayerIndex, const size_t nodeInNextLayerIndex) const
	{
		return weights[(nodeInNextLayerIndex*nextLayerSize) + nodeInNextLayerIndex];
	}
}
