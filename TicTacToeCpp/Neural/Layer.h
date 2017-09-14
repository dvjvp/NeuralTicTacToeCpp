#pragma once

namespace Neural
{
	template<typename T>
	class Layer
	{
	public:
		Layer(size_t thisLayerSize, size_t nextLayerSize);
		~Layer();

		void ClearInputs();

		inline T GetWeight(size_t nodeInThisLayerIndex, size_t nodeInNextLayerIndex);
	public:
		T* tresholds;
		T* weights;
		T* inputs;

		size_t thisLayerSize;
		size_t nextLayerSize;
	};
}

