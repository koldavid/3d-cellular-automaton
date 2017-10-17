#include "cudahelper.h"

#include <iostream>

void cudaErrorCheck(cudaError_t err)
{
	if (err != cudaSuccess) {
		std::cerr << "CUDA Error: " << cudaGetErrorString(err) << std::endl;
	}
}

