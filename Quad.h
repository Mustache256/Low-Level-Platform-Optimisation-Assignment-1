#pragma once
#include <iostream>
#include <cmath>
#include <vector>

#include "global.h"
#include "Box.h"

class Quad
{
	float topLeftX;
	float topLeftZ;
	float botRightX;
	float botRightZ;

	float minQuadSize;

	std::vector<Box*> boxesInQuad;

	Quad* topLeftTree;
	Quad* topRightTree;
	Quad* botLeftTree;
	Quad* botRightTree;

public:

	Quad(float minSize);
	Quad(float topLeft_x, float topleft_z, float botRight_x, float botRight_z, float mixSize);
	~Quad();
	void Insert(Box* box);
	std::vector<Box*> Search(float x, float z);
	bool InBoundary(float x, float z);
	float AddPositionsAndDivide(float pos1, float pos2);
	void UpdateQuadtree();

	void* operator new(size_t size);
};

