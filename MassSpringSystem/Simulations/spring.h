#ifndef UTIL_SPRING_H
#define UTIL_SPRING_H
#include "point.h"
#include "MassSpringSystemSimulator.h"

class spring {
public:
	int point1;
	int point2;
	float initialLength;

	// suggested in lecture but not used yet
	/*
	float stiffness;
	float currentLength;
	*/
};

#endif