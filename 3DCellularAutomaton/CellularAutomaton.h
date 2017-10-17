#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "Mat3D.h"

class CellularAutomaton
{
public:
	struct Rule {
		Rule(const std::vector<int>& survival,
			 const std::vector<int>& birth,
			 int numStates, 
			 std::string name);

		std::vector<int> survival;
		std::vector<int> birth;
		int numStates;

		std::string name;

		int* d_survival; // for gpu device
		int* d_birth; // for gpu device
	};

	CellularAutomaton(int cubeDimension, Rule rule);
	~CellularAutomaton();

	void initialize(int initialCubeSize, float initialCubeDensity);
	void updateCells();
	void updateCells_CUDA();
	const int* getDataPtr() const;
	int getCellCount() const;
	int getDimension() const;
	int getNumberOfStates() const;
	Mat3D<int>& getCells();


private:

	Mat3D<glm::ivec3> getNeighbourCoordinates(
		const glm::ivec3& center, 
		const glm::ivec3& boundaries) const;

	int cubeDimension;

	Mat3D<int> cells;
	Mat3D<int> previousGenerationCells;

	Rule rule;
};
