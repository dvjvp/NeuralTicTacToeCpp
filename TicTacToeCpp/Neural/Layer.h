#pragma once

/************************************************************************/
/*                              DECLARATIONS                            */
/************************************************************************/

namespace Neural
{
	/*
	* A single neural network layer.
	* It should probably be just a container for its neurons, but i figured
	* it'd be just easier if I'd be just storing theoretical neurons' values
	* directly in a layer in buffers.
	*/
	template<typename T>
	class Layer
	{
	public:
		/**
		* @brief Default constructor
		*
		* Does nothing. Actual initialisation is performed by Initialize() method and
		* it should be called right after constructing a layer. That way we can create
		* array of layers using @c new operator!
		*/
		Layer();
		/**
		* @brief Actual array initialization. Basically a second c-tor.
		* 
		* Allocates buffers for neuron values. Doesn't clear them because why
		* waste time. You can clear it manually, fill it with random
		* values or copy values to it yourself if you need that. No need to
		* do the same thing twice.
		* @param thisLayerSize - how many neurons should be in this layer
		* @param nextLayerSize - how many neurons should be in next layer. 
		* Should be 0 for output layer
		*/
		void Initialize(const size_t thisLayerSize, const size_t nextLayerSize);
		/**
		* @brief Default destructor
		* 
		* frees buffers allocated in Initialize() function.
		*/
		~Layer();

		
		/**
		* @brief Clears input buffer
		* 
		* Fills input buffer with 0-s. Uses memset, so it should be pretty quick
		*/
		void ClearInputs();

		/**
		* @brief Gets weight from @p nodeInThisLayerIndex neuron here to @p nodeInNextLayerIndex neuron in next layer
		* 
		* It's just a wrapper so there's no need to unwrap value from 2d array (stored as singular array) by hand every time 
		* 
		* @param nodeInThisLayerIndex - index of node in this layer from which output should the weight be taken
		* @param nodeInNextLayerIndex - index of node in next layer (target of connection we want to know weight of)
		* @return weight of connection between neurons
		*/
		inline T GetWeight(const size_t nodeInThisLayerIndex, const size_t nodeInNextLayerIndex) const;
	public:
		//Buffer for tresholds (minimal values needed to pass signal forward by neuron).
		//There's one for each neuron, so there's (thisLayerSize) of them
		T* tresholds;
		//Buffer for weights (value by which to multiply the input before passing it forward) from all neurons
		//in this layer to all neurons in next layer. There's (nextLayerSize) of them for each neuron, so total
		// of (thisLayerSize*nextLayerSize).
		T* weights;
		//Buffer for temporary values used by network during computation.
		//There's one for each layer, so there's (thisLayerSize) of them
		T* inputs;

		//Number of neurons in this layer
		size_t thisLayerSize;
		//Number of neurons in next layer. It's 0 if this is the output layer
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
		memset(inputs, 0, thisLayerSize * sizeof(T));
	}
	template<typename T>
	inline T Layer<T>::GetWeight(const size_t nodeInThisLayerIndex, const size_t nodeInNextLayerIndex) const
	{
		return weights[(nodeInNextLayerIndex*nextLayerSize) + nodeInNextLayerIndex];
	}
}
