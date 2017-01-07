#include "UniformGridCollision.h"

UniformGridCollision::UniformGridCollision(int cellSpacing, int maxObjects, int screenWidth, int screenHeight, int screenDepth)
{
	m_cellSpacing			= cellSpacing;
	m_maxObjects			= maxObjects;
	m_screenWidth			= screenWidth;
	m_screenHeight			= screenHeight;
	m_screenscreenDepth		= screenDepth;
	
	initGrid(); //init the grid
}

UniformGridCollision::UniformGridCollision()
{
	m_cellSpacing		= (0);
	m_maxObjects		= (0);
	m_screenWidth		= (0);
	m_screenHeight		= (0);
	m_screenscreenDepth	= (0);
}

UniformGridCollision::~UniformGridCollision()
{
	delete[] m_grid;
}

void UniformGridCollision::initGrid()
{
	float cellSpacing = m_cellSpacing < 1.0f ? 1.0f : m_cellSpacing;
	m_m =				m_maxObjects * (m_screenWidth/ cellSpacing)
									 * (m_screenHeight/ cellSpacing)
									 * (m_screenscreenDepth / cellSpacing)
									 / m_screenWidth; //for smaller approximation
	m_m = (m_maxObjects * (m_screenWidth)
		* (m_screenHeight )
		* (m_screenscreenDepth))
		/ m_cellSpacing; // the cells are supposed to be as big as 2*r 

	std::cout << "\nbytes = " << m_m * sizeof(byte) << "\n\n";

	m_grid = new byte[m_m];
}

void UniformGridCollision::updateGrid(std::vector<SphereSystem::Sphere> spheres)
{
	//first clear the array IT CAN BE EXPENSIVE
	//memset(m_grid, 0, sizeof(m_grid));
	unsigned int gridIndex = 0;
	for (SphereSystem::Sphere& s : spheres) {
		gridIndex = getArrayIndex(s);
		//std::cout << "gridIndex = " << gridIndex << "\tpos = "<<s.pos << endl;
	}

}

void UniformGridCollision::checkCollisions(SphereSystem::Sphere s1, SphereSystem::Sphere s2)
{

}


/*
	Here we get the array index of the m_grid array. It has to be substracted
	by the cell spacing to get the right index in the array and add with the sphere
	index to avoid collisions in the cell
*/
unsigned int UniformGridCollision::getArrayIndex(SphereSystem::Sphere s)
{
	unsigned int idx = 0;
	long double idxF= (s.pos.x + m_screenWidth * (s.pos.y + m_screenscreenDepth*s.pos.z)) / m_cellSpacing;
	//if its negative return just the positive value of it
	idx = (unsigned long) (idxF < 0? idxF - 2*idxF: idxF);
	return (unsigned long)(idx + s.ID);
}


