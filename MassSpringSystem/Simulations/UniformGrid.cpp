#include "UniformGrid.h"

UniformGrid::UniformGrid(float boxSize, int cellsPerDimension, int maxSpheresPerCell, float sphereRadius)
	: m_boxHalfSize(boxSize), m_cellsPerDimension(cellsPerDimension), m_maxSpheresPerCell(maxSpheresPerCell), m_sphereRadius(sphereRadius)
{
	int totalCells = cellsPerDimension * cellsPerDimension * cellsPerDimension;
	m_grid = new const Sphere*[maxSpheresPerCell * totalCells];
	m_occupiedCounts = new int[totalCells];
}

void UniformGrid::clearCells()
{
	std::fill(m_grid, m_grid + (m_maxSpheresPerCell * m_cellsPerDimension * m_cellsPerDimension * m_cellsPerDimension), nullptr);
	std::fill(m_occupiedCounts, m_occupiedCounts + (m_cellsPerDimension * m_cellsPerDimension * m_cellsPerDimension), 0);
}

void UniformGrid::addToCell(int x, int y, int z, const Sphere * sphere)
{
	if (x < 0 || x >= m_cellsPerDimension || y < 0 || y >= m_cellsPerDimension || z < 0 || z >= m_cellsPerDimension)
	{
		std::cout << "WARNING: object outside grid (" << x << ", " << y << ", " << z << ")" << std::endl;
		return;
	}

	const Sphere** cell = getCellByIndex(x, y, z);
	int* occupied = getOccupiedByIndex(x, y, z);

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
				}
			}
		}
	}
}


const std::unordered_set<std::pair<Sphere*, Sphere*>>& UniformGrid::computeCollisionPairs()
{
	m_collidingPairs.clear();

	for (int x = 0; x < m_cellsPerDimension; x++)
	{
		for (int y = 0; y < m_cellsPerDimension; y++)
		{
			for (int z = 0; z < m_cellsPerDimension; z++)
			{
				const Sphere** cell = getCellByIndex(x, y, z);
				int occupied = *getOccupiedByIndex(x, y, z);

				for (int i = 0; i < occupied; i++)
				{
					for (int j = i + 1; j < occupied; j++)
					{
						// i feel dirty on the inside
						Sphere* a = const_cast<Sphere*>(cell[i]);
						Sphere* b = const_cast<Sphere*>(cell[j]);

						std::pair<Sphere*, Sphere*> pair(a, b);
						m_collidingPairs.insert(pair);
					}
				}
			}
		}
	}

	return m_collidingPairs;
}


const Sphere** UniformGrid::getCellByIndex(int x, int y, int z)
{
	int c = m_maxSpheresPerCell;
	int n = m_cellsPerDimension;
	return m_grid + (c * (z + n*y + n*n*x));
}

int* UniformGrid::getOccupiedByIndex(int x, int y, int z)
{
	int n = m_cellsPerDimension;
	return m_occupiedCounts + (z + n*y + n*n*x);
}
