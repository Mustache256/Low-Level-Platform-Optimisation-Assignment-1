#pragma once
#include "Vector3.h"
#include "Tracker.h"
#include "global.h"

class Box
{
public:
	Box() {};
	~Box() {};

	Vector3 position;
	Vector3 size;
	Vector3 velocity;
	Vector3 colour;

	void genRandPos(Box* box);
	void genRandVel(Box* box);
	void genRandCol(Box* box);
	void setBoxSize(Box* box, float x, float y, float z);

	void* operator new(size_t size);
};

