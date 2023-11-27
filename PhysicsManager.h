#pragma once
#include <cmath>
#include <iostream>
#include <chrono>

#include "Common.h"
#include "Constants.h"
#include "Vector3.h"
#include "Tracker.h"
#include "Global.h"

using namespace std;

class PhysicsManager
{
public:
	PhysicsManager(float newGravity, float newFloorY);
	~PhysicsManager() {};

	void ApplyGravity(float& objY);
	void ApplyVelocityChange(Vector3& objPos, Vector3& objVel);

	void CheckBoundsCollision(Vector3& objPos, Vector3& objVel, Vector3& objSize);
	bool CheckOtherCollision(Vector3& objPos, Vector3& objSize, Vector3& otherPos, Vector3& otherSize);
	void ResolveOtherCollision(Vector3& objPos, Vector3& objVel, Vector3& otherPos, Vector3& otherVel);

	void UpdateDeltaTime();

	void* operator new(size_t size);

private:
	float gravity;
	float deltaTime;
	float floorY;
};

