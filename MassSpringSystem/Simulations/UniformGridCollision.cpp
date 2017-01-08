#include "UniformGridCollision.h"

UniformGridCollision::UniformGridCollision(int cellSpacing, int maxObjects, int screenWidth, int screenHeight, int screenDepth)
{
	m_cellSpacing			= cellSpacing;
	m_maxObjects			= maxObjects;
m_screenWidth = screenWidth;
m_screenHeight = screenHeight;
m_screenscreenDepth = screenDepth;

initGrid(); //init the grid
}

UniformGridCollision::UniformGridCollision()
{
	m_cellSpacing = (0);
	m_maxObjects = (0);
	m_screenWidth = (0);
	m_screenHeight = (0);
	m_screenscreenDepth = (0);
}

UniformGridCollision::~UniformGridCollision()
{
	delete[] m_grid;
	delete[] m_spheresGridPositions;
}

void UniformGridCollision::initGrid()
{
	float cellSpacing = m_cellSpacing < 1.0f ? 1.0f : m_cellSpacing;
	m_m = m_maxObjects * (m_screenWidth / cellSpacing)
		* (m_screenHeight / cellSpacing)
		* (m_screenscreenDepth / cellSpacing)
		/ m_screenWidth; //for smaller approximation
//m_m = (m_maxObjects * (m_screenWidth)
	m_m = (unsigned int) ((m_screenWidth)
		* (m_screenHeight)
		* (m_screenscreenDepth))
		/ m_cellSpacing; // the cells are supposed to be as big as 2*r 

	std::cout << "\ngrid array size in bytes -> " << m_m  << "\n\n";

	//grid values
	m_grid = new char[m_m];
	//holds where each sphere in the grid array are saved
	m_spheresGridPositions = new char[m_maxObjects];
	//holds the colliding pairs of spheres. It with this size just to make everything works in O(n) and not n+m
	//m_colPairs.reserve(m_maxObjects + m_maxObjects);
}

void UniformGridCollision::updateGrid(std::vector<SphereSystem::Sphere> spheres)
{
	unsigned int gridIndex = 0;

	for (SphereSystem::Sphere& s : spheres) {
		gridIndex = getArrayIndex(s);
		std::cout << "gridIndex=" << gridIndex << std::endl;

		//save where the object is saved
		*(m_spheresGridPositions + s.ID) = gridIndex;

		//update the grid value if its not presented
		if (*(m_grid + gridIndex) == 0) {
			//empty cell so just add the index of the object to this cell
			*(m_grid + gridIndex) = s.ID;
		}
		//for some reason it is the same element
		else if (*(m_grid + gridIndex) == s.ID)
			continue;
		else if (*(m_grid + gridIndex) != s.ID) { //collision with oder element
			//add the colliding pairs. First object is the object saved there and the second is the current
			
			//TODO FIX IT OUT OF MEMORY
			addCollidingPair(spheres[*(m_grid + gridIndex)], s);
			
			//update the current index in the grid
			*(m_grid + gridIndex) = s.ID;
		}

	}

}

std::vector<UniformGridCollision::CollidingSpheresPair> UniformGridCollision::getCollidingPairs()
{
	return m_colPairs;
}

/*
	Here we get the array index of the m_grid array. It has to be substracted
	by the cell spacing to get the right index in the array and add with the sphere
	index to avoid collisions in the cell
*/
unsigned int UniformGridCollision::getArrayIndex(SphereSystem::Sphere s)
{
	unsigned int idx = 0;
	long double idxF = (s.pos.x + m_screenWidth * (s.pos.y + m_screenscreenDepth*s.pos.z)) / m_cellSpacing;
	//if its negative return just the positive value of it
	idx = (unsigned long)(idxF < 0 ? idxF - 2 * idxF : idxF);
	return (unsigned long)(idx + s.ID);
}

void UniformGridCollision::addCollidingPair(SphereSystem::Sphere s1, SphereSystem::Sphere s2)
{
	CollidingSpheresPair cp;
	cp.s1 = s1;
	cp.s2 = s2;

	//try to map in on the collidingspair vector as simple as possible
	unsigned int saveIndex = s1.ID + s2.ID;
	//check if this index is set
	if (saveIndex > m_colPairs.size()) {
		//it is new for the vector so just add it
		m_colPairs[saveIndex] = cp;
	}
	//else it is already set
	else {
		//duplicates
		if (m_colPairs[saveIndex].s1.ID == s1.ID || m_colPairs[saveIndex].s2.ID == s1.ID || m_colPairs[saveIndex].s1.ID == s2.ID || m_colPairs[saveIndex].s2.ID == s2.ID) {
			//ignore this case becase it is(are) the same element(s)
			return;
		}
		// check for false collision -> if the current element in the grid array is out dated
		else if (getArrayIndex(s1) != getArrayIndex(s2)) {
			//ignore this case 
			return;
		}
		//else it is a real collision between the two spheres
		//it is ID pairs from type 4+2 and 1+7 then just find next free element to save it
		else {
			for (unsigned int i = saveIndex; i < m_colPairs.size(); i++) {
				//check if its free
				if (m_colPairs[i].s1.ID == NULL && m_colPairs[i].s2.ID == NULL) {
					m_colPairs[i] = cp;
					return;
				}
			}
			//if it reach here => add it at the end of the vector
			m_colPairs[m_colPairs.size()] = cp;
		}
	}
	
}



