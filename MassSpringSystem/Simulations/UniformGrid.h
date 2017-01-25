#ifndef UNIFORMGRID_h
#define UNIFORMGRID_h

#include <vector>
#include <unordered_set>
#include "Sphere.h"

#define GRID_RESOLUTION 

// absolutely bonkers c++ way of providing a custom hash function for unordered_set
namespace std {
	template <>
	class hash<std::pair<Sphere*, Sphere*>> {
	public:
		size_t operator()(const std::pair<Sphere*, Sphere*>& pair) const
		{
			return hash<void*>()(pair.first) ^ hash<void*>()(pair.second);
		}
	};
};


class UniformGrid {
public:
	UniformGrid(float boxSize, int cellsPerDimension, int maxSpheresPerCell);

	void updateGrid(const std::vector<Sphere>& spheres);
	const std::unordered_set<std::pair<Sphere*, Sphere*>>& computeCollisionPairs();

private:
	float m_boxHalfSize;
	int m_cellsPerDimension;
	int m_maxSpheresPerCell;

	// stores pointers to spheres in each cell; constant size of m_maxSpheresPerCell * m_cellsPerDimension^3
	std::vector<const Sphere*> m_gridV;
	// stores amount of occupied slots per cell; array of size m_cellsPerDimension^3
	std::vector<int> m_occupiedCountsV;

	std::unordered_set<std::pair<Sphere*, Sphere*>> m_collidingPairs;

	void clearCells();
	void addToCell(int x, int y, int z, const Sphere* sphere);
	std::vector<const Sphere*>::iterator getCellByIndex(int x, int y, int z);
	std::vector<int>::iterator getOccupiedByIndex(int x, int y, int z);
};

#endif