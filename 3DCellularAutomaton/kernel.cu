#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>
#include <device_functions.h>
#include <cuda.h>

#include <iostream>

#include "CellularAutomaton.h"
#include "cudahelper.h"

struct d_Rule
{
	int* survival;
	int* birth;
	int numStates;
};

// cubeDimension^3 blocks, each block with 27 threads, threads check the current cell's neighbours
__global__ void calculateNextGeneration(int* current, int* next, d_Rule rule)
{
	__shared__ int neighbours[3 * 3 * 3];

	int neighboursIndex = threadIdx.x * 3 * 3 + threadIdx.y * 3 + threadIdx.z;
	neighbours[neighboursIndex] = 0;

	int neighbourCoordinate_x = blockIdx.x + (threadIdx.x - 1);
	int neighbourCoordinate_y = blockIdx.y + (threadIdx.y - 1);
	int neighbourCoordinate_z = blockIdx.z + (threadIdx.z - 1);

	if (neighbourCoordinate_x < 0) {
		neighbourCoordinate_x = gridDim.x - 1;
	}
	if (neighbourCoordinate_y < 0) {
		neighbourCoordinate_y = gridDim.y - 1;
	}
	if (neighbourCoordinate_z < 0) {
		neighbourCoordinate_z = gridDim.z - 1;
	}

	if (neighbourCoordinate_x >= gridDim.x) {
		neighbourCoordinate_x = 0;
	}
	if (neighbourCoordinate_y >= gridDim.y) {
		neighbourCoordinate_y = 0;
	}
	if (neighbourCoordinate_z >= gridDim.z) {
		neighbourCoordinate_z = 0;
	}

	int neighbourLocation =
		neighbourCoordinate_x * (gridDim.x*gridDim.x) +
		neighbourCoordinate_y * (gridDim.x) +
		neighbourCoordinate_z;

	int cellLocation =
		blockIdx.x * (gridDim.x*gridDim.x) +
		blockIdx.y * (gridDim.x) +
		blockIdx.z;

	if (cellLocation != neighbourLocation) {
		if (current[neighbourLocation] == rule.numStates - 1) {
			neighbours[neighboursIndex] = 1;
		}

		__syncthreads();

		if (cellLocation == neighbourLocation) { // run only on one thread
			int neighbourCount = 0;
			for (int i = 0; i < 3 * 3 * 3; i++)
			{
				neighbourCount += neighbours[i];
			}

			if (current[cellLocation] > 0) { // alive cell
				if (rule.survival[neighbourCount] != 1) {
					next[cellLocation] = current[cellLocation] - 1; // aging
				}
			}
			else { // dead cell
				if (rule.birth[neighbourCount] == 1) {
					next[cellLocation] = rule.numStates - 1; // new birth
				}
			}
		}
	}
}


// (cubeDimension/10)^3 blocks with 10*10*10=1000 threads per block, each thread operates on a single cell
__global__ void calculateNextGeneration_updated(int* current, int* next, d_Rule rule)
{
	int xToID = gridDim.x*gridDim.x*blockDim.x*blockDim.x;
	int yToID = gridDim.x*blockDim.x;
	int zToID = 1;

	int dimMax = (gridDim.x - 1)*blockDim.x + (blockDim.x - 1);
	int xMax = dimMax * xToID;
	int yMax = dimMax * yToID;
	int zMax = dimMax * zToID;

	int xCoord = (blockIdx.x*blockDim.x + threadIdx.x);
	int yCoord = (blockIdx.y*blockDim.x + threadIdx.y);
	int zCoord = (blockIdx.z*blockDim.x + threadIdx.z);

	int x = xCoord * xToID;
	int y = yCoord * yToID;
	int z = zCoord * zToID;

	int _x = (x > 0 ? (xCoord - 1)*xToID : xMax); // x minus 1
	int _y = (y > 0 ? (yCoord - 1)*yToID : yMax); // y minus 1
	int _z = (z > 0 ? (zCoord - 1)*zToID : zMax); // z minus 1

	int x_ = (x < xMax ? (xCoord + 1)*xToID : 0); // x plus 1
	int y_ = (y < yMax ? (yCoord + 1)*yToID : 0); // y plus 1
	int z_ = (z < zMax ? (zCoord + 1)*zToID : 0); // z plus 1

	int cellLocation = x + y + z;

	int neighbourCount = 0;
	if (current[_x + _y + _z] == rule.numStates - 1) neighbourCount++;
	if (current[_x + _y + z]  == rule.numStates - 1) neighbourCount++;
	if (current[_x + _y + z_] == rule.numStates - 1) neighbourCount++;
	if (current[_x + y + _z]  == rule.numStates - 1) neighbourCount++;
	if (current[_x + y + z]   == rule.numStates - 1) neighbourCount++;
	if (current[_x + y + z_]  == rule.numStates - 1) neighbourCount++;
	if (current[_x + y_ + _z] == rule.numStates - 1) neighbourCount++;
	if (current[_x + y_ + z]  == rule.numStates - 1) neighbourCount++;
	if (current[_x + y_ + z_] == rule.numStates - 1) neighbourCount++;

	if (current[x + _y + _z]  == rule.numStates - 1) neighbourCount++;
	if (current[x + _y + z]   == rule.numStates - 1) neighbourCount++;
	if (current[x + _y + z_]  == rule.numStates - 1) neighbourCount++;
	if (current[x + y + _z]   == rule.numStates - 1) neighbourCount++;
	if (current[x + y + z_]   == rule.numStates - 1) neighbourCount++;
	if (current[x + y_ + _z]  == rule.numStates - 1) neighbourCount++;
	if (current[x + y_ + z]   == rule.numStates - 1) neighbourCount++;
	if (current[x + y_ + z_]  == rule.numStates - 1) neighbourCount++;

	if (current[x_ + _y + _z] == rule.numStates - 1) neighbourCount++;
	if (current[x_ + _y + z]  == rule.numStates - 1) neighbourCount++;
	if (current[x_ + _y + z_] == rule.numStates - 1) neighbourCount++;
	if (current[x_ + y + _z]  == rule.numStates - 1) neighbourCount++;
	if (current[x_ + y + z]   == rule.numStates - 1) neighbourCount++;
	if (current[x_ + y + z_]  == rule.numStates - 1) neighbourCount++;
	if (current[x_ + y_ + _z] == rule.numStates - 1) neighbourCount++;
	if (current[x_ + y_ + z]  == rule.numStates - 1) neighbourCount++;
	if (current[x_ + y_ + z_] == rule.numStates - 1) neighbourCount++;


	if (current[cellLocation] > 0) { // alive cell
		if (rule.survival[neighbourCount] != 1) {
			next[cellLocation] = current[cellLocation] - 1; // aging
		}
	}
	else { // dead cell
		if (rule.birth[neighbourCount] == 1) {
			next[cellLocation] = rule.numStates - 1; // new birth
		}
	}
}


void CellularAutomaton::updateCells_CUDA()
{
	previousGenerationCells = cells;

	int* d_current;
	int* d_next;

	cudaErrorCheck(cudaMalloc(&d_current, sizeof(int) * this->getCellCount()));
	cudaErrorCheck(cudaMalloc(&d_next, sizeof(int) * this->getCellCount()));

	cudaErrorCheck(cudaMemcpy(d_current, previousGenerationCells.getDataPtr(), sizeof(int) * this->getCellCount(), cudaMemcpyHostToDevice));
	cudaErrorCheck(cudaMemcpy(d_next, cells.getDataPtr(), sizeof(int) * this->getCellCount(), cudaMemcpyHostToDevice));

	d_Rule d_rule;
	cudaErrorCheck(cudaMalloc(&(d_rule.birth), sizeof(int) * 27));
	cudaErrorCheck(cudaMalloc(&(d_rule.survival), sizeof(int) * 27));

	cudaErrorCheck(cudaMemcpy(d_rule.birth, rule.d_birth, sizeof(int) * 27, cudaMemcpyHostToDevice));
	cudaErrorCheck(cudaMemcpy(d_rule.survival, rule.d_survival, sizeof(int) * 27, cudaMemcpyHostToDevice));

	d_rule.numStates = rule.numStates;

	/*dim3 numBlocks(cubeDimension, cubeDimension, cubeDimension);
	dim3 numThreadsPerBlock(3, 3, 3);
	calculateNextGeneration <<< numBlocks, numThreadsPerBlock >>> (d_current, d_next, d_rule);*/

	int blockDim = cubeDimension / 10;
	dim3 numBlocks(blockDim, blockDim, blockDim);
	dim3 numThreadsPerBlock(10, 10, 10); // 1000 threads per block!!!
	calculateNextGeneration_updated <<< numBlocks, numThreadsPerBlock >>> (d_current, d_next, d_rule);


	// Checking failures on kernel
	auto cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		std::cerr << "CUDA kernel failed: " << cudaGetErrorString(cudaStatus) << std::endl;
	}

	cudaErrorCheck(cudaDeviceSynchronize());

	cudaErrorCheck(cudaMemcpy(cells.getDataPtr(), d_next, sizeof(int) * this->getCellCount(), cudaMemcpyDeviceToHost));

	cudaFree(d_current);
	cudaFree(d_next);
}