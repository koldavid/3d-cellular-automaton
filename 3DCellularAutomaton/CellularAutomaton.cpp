#include "CellularAutomaton.h"

#include <random>
#include <iostream>
#include <iomanip>

CellularAutomaton::CellularAutomaton(int cubeDimension, Rule rule) : 
	cubeDimension(cubeDimension),
	cells(cubeDimension),
	previousGenerationCells(cubeDimension),
	rule(rule)
{
}


CellularAutomaton::~CellularAutomaton()
{
}

void CellularAutomaton::initialize(int initialCubeSize, float initialCubeDensity)
{
	int min = static_cast<int>((cubeDimension + initialCubeSize) / 2.0 - initialCubeSize);
	int max = static_cast<int>((cubeDimension + initialCubeSize) / 2.0);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);

	int numCells = 0;

	for (int i = 0; i < cubeDimension; i++) { 
		for (int j = 0; j < cubeDimension; j++) {
			for (int k = 0; k < cubeDimension; k++) {
				if (i > min && i<max &&
					j > min && j<max &&
					k > min && k<max) {
					cells[i][j][k] = (dis(gen) < initialCubeDensity ? rule.numStates - 1 : 0);
					if (cells[i][j][k] != 0) numCells++;
				}
				else {
					cells[i][j][k] = 0;
				}
			}
		}
	}

	std::cout << "\nCellular automaton initialized" << std::endl
	 << "  -cube size: " << cubeDimension << "*" << cubeDimension << "*" << cubeDimension << "=" << cubeDimension*cubeDimension*cubeDimension << std::endl
	 << "  -initial cube size: " << initialCubeSize << "*" << initialCubeSize << "*" << initialCubeSize << "=" << initialCubeSize*initialCubeSize*initialCubeSize << std::endl
	 << "  -initial cube density: " << std::setprecision(4) << (100.0f*numCells) / (initialCubeSize*initialCubeSize*initialCubeSize) << "%" << std::endl
	 << " Rule: " << rule.name << std::endl
	 << "  -survival: ";
	for (auto it : rule.survival) std::cout << it << " "; 
	std::cout << "\n  -birth: ";
	for (auto it : rule.birth) std::cout << it << " ";
	std::cout << "\n  -number of states: " << rule.numStates << std::endl;

}

void CellularAutomaton::updateCells()
{
	previousGenerationCells = cells;

	for (int i = 0; i < cubeDimension; i++) {
		for (int j = 0; j < cubeDimension; j++) {
			for (int k = 0; k < cubeDimension; k++) {
				int neighbourCount = 0;

				glm::ivec3 nullCell = glm::ivec3(-1, -1, -1);
				glm::ivec3 currentCell = glm::ivec3(i, j, k);
				Mat3D<glm::ivec3> neighbourCoordinates =
					getNeighbourCoordinates(
						currentCell,
						glm::ivec3(cubeDimension, cubeDimension, cubeDimension));

				for (int ii = 0; ii < 3; ii++) {
					for (int jj = 0; jj < 3; jj++) {
						for (int kk = 0; kk < 3; kk++) {
							if (neighbourCoordinates[ii][jj][kk] != nullCell &&
								neighbourCoordinates[ii][jj][kk] != currentCell) {
								if (previousGenerationCells
									[neighbourCoordinates[ii][jj][kk].x]
								[neighbourCoordinates[ii][jj][kk].y]
								[neighbourCoordinates[ii][jj][kk].z]
								!= 0) {
									neighbourCount++;
								}
							}
						}
					}
				}

				if (previousGenerationCells[i][j][k] != 0) {
					bool survive = false;
					for (auto it : rule.survival) {
						if (neighbourCount == it) {
							survive = true;
							break;
						}
					}

					if (!survive) {
						cells[i][j][k] = previousGenerationCells[i][j][k] - 1;
					}
				}
				else {
					bool birth = false;
					for (auto it : rule.birth) {
						if (neighbourCount == it) {
							birth = true;
							break;
						}
					}

					if (birth) {
						cells[i][j][k] = rule.numStates-1;
					}
				}
			}
		}
	}
}

Mat3D<int>& CellularAutomaton::getCells()
{
	return cells;
}

const int * CellularAutomaton::getDataPtr() const
{
	return cells.getDataPtr();
}

int CellularAutomaton::getCellCount() const
{
	return cubeDimension*cubeDimension*cubeDimension;
}

int CellularAutomaton::getDimension() const
{
	return cubeDimension;
}

int CellularAutomaton::getNumberOfStates() const
{
	return rule.numStates;
}

CellularAutomaton::Rule::Rule(
	const std::vector<int>& survival, 
	const std::vector<int>& birth, 
	int numStates, std::string name)
{
	this->name = name;
	this->survival = std::vector<int>(survival);
	this->birth = std::vector<int>(birth);
	this->numStates = numStates;

	this->d_birth = new int[27];
	this->d_survival = new int[27];

	for(int i = 0; i < 27; i++) {
		this->d_birth[i] = 0;
		this->d_survival[i] = 0;
	}

	for (auto it : birth) {
		this->d_birth[it] = 1;
	}

	for (auto it : survival) {
		this->d_survival[it] = 1;
	}
}


Mat3D<glm::ivec3> CellularAutomaton::getNeighbourCoordinates(const glm::ivec3& center, const glm::ivec3& boundaries) const
{
	Mat3D<glm::ivec3> coords(3);

	coords[0][0][0] = glm::ivec3(center.x - 1, center.y - 1, center.z - 1);
	coords[1][0][0] = glm::ivec3(center.x, center.y - 1, center.z - 1);
	coords[2][0][0] = glm::ivec3(center.x + 1, center.y - 1, center.z - 1);

	coords[0][1][0] = glm::ivec3(center.x - 1, center.y, center.z - 1);
	coords[1][1][0] = glm::ivec3(center.x, center.y, center.z - 1);
	coords[2][1][0] = glm::ivec3(center.x + 1, center.y, center.z - 1);

	coords[0][2][0] = glm::ivec3(center.x - 1, center.y + 1, center.z - 1);
	coords[1][2][0] = glm::ivec3(center.x, center.y + 1, center.z - 1);
	coords[2][2][0] = glm::ivec3(center.x + 1, center.y + 1, center.z - 1);

	coords[0][0][1] = glm::ivec3(center.x - 1, center.y - 1, center.z);
	coords[1][0][1] = glm::ivec3(center.x, center.y - 1, center.z);
	coords[2][0][1] = glm::ivec3(center.x + 1, center.y - 1, center.z);

	coords[0][1][1] = glm::ivec3(center.x - 1, center.y, center.z);
	coords[1][1][1] = glm::ivec3(center.x, center.y, center.z);
	coords[2][1][1] = glm::ivec3(center.x + 1, center.y, center.z);

	coords[0][2][1] = glm::ivec3(center.x - 1, center.y + 1, center.z);
	coords[1][2][1] = glm::ivec3(center.x, center.y + 1, center.z);
	coords[2][2][1] = glm::ivec3(center.x + 1, center.y + 1, center.z);

	coords[0][0][2] = glm::ivec3(center.x - 1, center.y - 1, center.z + 1);
	coords[1][0][2] = glm::ivec3(center.x, center.y - 1, center.z + 1);
	coords[2][0][2] = glm::ivec3(center.x + 1, center.y - 1, center.z + 1);

	coords[0][1][2] = glm::ivec3(center.x - 1, center.y, center.z + 1);
	coords[1][1][2] = glm::ivec3(center.x, center.y, center.z + 1);
	coords[2][1][2] = glm::ivec3(center.x + 1, center.y, center.z + 1);

	coords[0][2][2] = glm::ivec3(center.x - 1, center.y + 1, center.z + 1);
	coords[1][2][2] = glm::ivec3(center.x, center.y + 1, center.z + 1);
	coords[2][2][2] = glm::ivec3(center.x + 1, center.y + 1, center.z + 1);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				if (coords[i][j][k].x == -1) {
					coords[i][j][k].x = boundaries.x - 1;
				}
				if (coords[i][j][k].x == boundaries.x) {
					coords[i][j][k].x = 0;
				}

				if (coords[i][j][k].y == -1) {
					coords[i][j][k].y = boundaries.y - 1;
				}
				if (coords[i][j][k].y == boundaries.y) {
					coords[i][j][k].y = 0;
				}

				if (coords[i][j][k].z == -1) {
					coords[i][j][k].z = boundaries.z - 1;
				}
				if (coords[i][j][k].z == boundaries.z) {
					coords[i][j][k].z = 0;
				}
			}
		}
	}

	return coords;
}
