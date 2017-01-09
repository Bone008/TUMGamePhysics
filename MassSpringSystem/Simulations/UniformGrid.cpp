#include "UniformGrid.h"

UniformGrid::UniformGrid(float boxSize, int cellsPerDim, int maxSpheresPerCell, float sphereRadius)
	: m_boxHalfSize(boxSize), m_cellsPerDimension(cellsPerDim), m_maxSpheresPerCell(maxSpheresPerCell), m_sphereRadius(sphereRadius),
	m_gridV(maxSpheresPerCell * cellsPerDim * cellsPerDim * cellsPerDim, nullptr),
	m_occupiedCountsV(cellsPerDim * cellsPerDim * cellsPerDim, 0)
{
	std::cout << "sphere diameter = " << (2 * sphereRadius) << "; grid size = " << (2.0 * m_boxHalfSize / m_cellsPerDimension) << std::endl;
	std::cout << "grid memory use = " << (m_gridV.size() * sizeof(Sphere*)) << " bytes" << std::endl;
}

void UniformGrid::clearCells()
{
	// we could zero out the pointers as well, but for performance reasons we don't as it is unnecessary
	//std::fill(m_gridV.begin(), m_gridV.end(), nullptr);
	
	// mark all cells as unoccupied
	std::fill(m_occupiedCountsV.begin(), m_occupiedCountsV.end(), 0);
}

void UniformGrid::addToCell(int x, int y, int z, const Sphere* sphere)
{
	if (x < 0 || x >= m_cellsPerDimension || y < 0 || y >= m_cellsPerDimension || z < 0 || z >= m_cellsPerDimension)
	{
		std::cout << "WARNING: object outside grid (" << x << ", " << y << ", " << z << ")" << std::endl;
		return;
	}

	std::vector<const Sphere*>::iterator cell = getCellByIndex(x, y, z);
	std::vector<int>::iterator occupied = getOccupiedByIndex(x, y, z);

	if (*occupied >= m_maxSpheresPerCell)
	{
		std::cout << "WARNING: cell is overflowing (" << x << ", " << y << ", " << z << ")" << std::endl;
		return;
	}

	cell[*occupied] = sphere;
	(*occupied)++;
}

void UniformGrid::updateGrid(const std::vector<Sphere>& spheres)
{
	clearCells();

	float cellSize = 2.0 * m_boxHalfSize / m_cellsPerDimension;
	// cells start at "top left" corner of the cube,
	// so cell (0, 0, 0) is not at location (0, 0, 0) but in the corner
	Vec3 cellsOrigin = Vec3(-m_boxHalfSize, -m_boxHalfSize, -m_boxHalfSize);

	int c = 0;
	for (const Sphere& sphere : spheres)
	{
		Vec3 localMin = sphere.pos - cellsOrigin - m_sphereRadius;
		Vec3 localMax = sphere.pos - cellsOrigin + m_sphereRadius;
		int xmin = floor(localMin.x / cellSize);
		int ymin = floor(localMin.y / cellSize);
		int zmin = floor(localMin.z / cellSize);
		int xmax = floor(localMax.x / cellSize);
		int ymax = floor(localMax.y / cellSize);
		int zmax = floor(localMax.z / cellSize);

		for (int x = xmin; x <= xmax; x++)
		{
			for (int y = ymin; y <= ymax; y++)
			{
				for (int z = zmin; z <= zmax; z++)
				{
					addToCell(x, y, z, &sphere);
					c++;
				}
			}
		}
	}
	//std::cout << "avg. cells per sphere: " << ((float)c / spheres.size()) << std::endl;
}


const std::unordered_set<std::pair<Sphere*, Sphere*>>& UniformGrid::computeCollisionPairs()
{
	m_collidingPairs.clear();
	int addAttempts = 0;

	for (int x = 0; x < m_cellsPerDimension; x++)
	{
		for (int y = 0; y < m_cellsPerDimension; y++)
		{
			for (int z = 0; z < m_cellsPerDimension; z++)
			{
				std::vector<const Sphere*>::iterator cell = getCellByIndex(x, y, z);
				int occupied = *getOccupiedByIndex(x, y, z);

				for (int i = 0; i < occupied; i++)
				{
					for (int j = i + 1; j < occupied; j++)
					{
						// i feel dirty on the inside
						Sphere* a = const_cast<Sphere*>(cell[i]);
						Sphere* b = const_cast<Sphere*>(cell[j]);

						m_collidingPairs.emplace(a, b);
						addAttempts++;
					}
				}
			}
		}
	}

	//std::cout << "pairs = " << m_collidingPairs.size() << "; attempts = " << addAttempts << std::endl;

	return m_collidingPairs;
}


std::vector<const Sphere*>::iterator UniformGrid::getCellByIndex(int x, int y, int z)
{
	int c = m_maxSpheresPerCell;
	int n = m_cellsPerDimension;
	return m_gridV.begin() + (c * (z + n*y + n*n*x));
}

std::vector<int>::iterator UniformGrid::getOccupiedByIndex(int x, int y, int z)
{
	int n = m_cellsPerDimension;
	return m_occupiedCountsV.begin() + (z + n*y + n*n*x);
}
