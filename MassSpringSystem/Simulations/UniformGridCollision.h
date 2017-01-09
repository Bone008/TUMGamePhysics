#ifndef UNIFORM_GRID_COLLISION_H
#define UNIFORM_GRID_COLLISION_H

#include "SphereSystem.h"

class UniformGridCollision {
public:
	struct CollidingSpheresPair
	{
		SphereSystem::Sphere s1;
		SphereSystem::Sphere s2;

	};
	UniformGridCollision(int cellSpacing,int maxObjects, int screenWidth, int screenHeight, int screenDepth);
	UniformGridCollision();
	~UniformGridCollision();

	float m_cellSpacing;		//in our case is h = 2*r
	int m_maxObjects;			//as described in exercise3 - max objects per cell
	int m_m;					//the number of cells in the grid
	int m_screenWidth;
	int m_screenHeight;
	int m_screenscreenDepth;

	byte* m_grid;					//the grid array. 1 for object in the cell and 0 otherwise
	byte* m_spheresGridPositions;	//here we store the positions of each sphere ID
	void initGrid();
	void updateGrid(std::vector<SphereSystem::Sphere> spheres);
	void checkCollisions(SphereSystem::Sphere s1, SphereSystem::Sphere s2);
	unsigned int getArrayIndex(SphereSystem::Sphere s);

};
#endif