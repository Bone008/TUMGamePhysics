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
private:
	Sphere* m_grid;
	int* m_occupiedCounts;

	std::vector<SpherePair> m_collidingPairs;
public:
	void updateGrid(const std::vector<Sphere>& spheres);
	const std::vector<SpherePair>& computeCollisionPairs();
};

#endif