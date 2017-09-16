#pragma once
#include <algorithm>
#include <numeric>
#include <vector>

/**
* @brief Sorts an array and returns array of indexes for sorted version
* 
* Copied from StackOverflow, lol
* Source:
* https://stackoverflow.com/questions/1577475/c-sorting-and-keeping-track-of-indexes
* @param Array to sort
* @return Array of indexes representing sorted values from original array
*/
template <typename T>
std::vector<size_t> sort_indexes(const std::vector<T> &v) {

	// initialize original index locations
	std::vector<size_t> idx(v.size());
	iota(idx.begin(), idx.end(), 0);

	// sort indexes based on comparing values in v
	sort(idx.begin(), idx.end(),
		[&v](size_t i1, size_t i2) {return v[i1] < v[i2]; });

	return idx;
}