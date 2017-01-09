#ifndef UNIFORMGRID_h
#define UNIFORMGRID_h

#include <vector>
#include "Sphere.h"

#define GRID_RESOLUTION 

struct SpherePair {
	Sphere& a;
	Sphere& b;

	SpherePair(Sphere& a, Sphere& b) : a(a), b(b) {}
};

class UniformGrid {
public:
	UniformGrid(float boxSize, int cellsPerDimension, int maxSpheresPerCell, float sphereRadius);

	void updateGrid(const std::vector<Sphere>& spheres);
	const std::vector<SpherePair>& computeCollisionPairs();

private:
	float m_sphereRadius;
	float m_boxHalfSize;
	int m_cellsPerDimension;
	int m_maxSpheresPerCell;

	const Sphere** m_grid; // stores pointers to spheres in each cell; array of size m_maxSpheresPerCell * m_cellsPerDimension^3
	int* m_occupiedCounts; // stores amount of occupied slots per cell; array of size m_cellsPerDimension^3

	std::vector<SpherePair> m_collidingPairs;

	void clearCells();
	void addToCell(int x, int y, int z, const Sphere* sphere);
	const Sphere** getCellByIndex(int x, int y, int z);
	int* getOccupiedByIndex(int x, int y, int z);
};

#endif