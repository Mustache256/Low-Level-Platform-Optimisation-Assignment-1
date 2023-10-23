#pragma once
#include "Vector3.h"

class Box
{
public:
	Vector3 position;
	Vector3 size;
	Vector3 velocity;
	Vector3 colour;

	void genRandPos(Box& box);
	void genRandVel(Box& box);
	void genRandCol(Box& box);
	void setBoxSize(Box& box, float x, float y, float z);
};

