#include "PhysicsManager.h"

PhysicsManager::PhysicsManager(float newGravity, const float newFloorY)
{
	//Assigns gravity and floorY values
	gravity = newGravity;
	floorY = newFloorY;
}

void PhysicsManager::ApplyGravity(float& objY)
{
	//Applying force of gravity to object
	objY += gravity * deltaTime;
}

void PhysicsManager::ApplyVelocityChange(Vector3& objPos, Vector3& objVel)
{
	//Applies velocity change over time to object
	objPos.x += objVel.x * deltaTime;
	objPos.y += objVel.y * deltaTime;
	objPos.z += objVel.z * deltaTime;
}

void PhysicsManager::CheckBoundsCollision(Vector3& objPos, Vector3& objVel, Vector3& objSize)
{
	//Checking for object collision with floor, if colliding apply response force to object
	if (objPos.y - objSize.y / 2.0f < floorY)
	{
		objPos.y = floorY + objSize.y / 2.0f;
		float dampening = 0.7f;
		objVel.y = -objVel.y * dampening;
	}

	//Cehck if object is colliding with sizes of scene, if colliding aplly response force to object
	if (objPos.x - objSize.x / 2.0f < minX || objPos.x + objSize.x / 2.0f > maxX)
	{
		objVel.x = -objVel.x;
	}

	if (objPos.z - objSize.z / 2.0f < minZ || objPos.z + objSize.z / 2.0f > maxZ)
	{
		objPos.z = -objPos.z;
	}
}

bool PhysicsManager::CheckOtherCollision(Vector3& objPos, Vector3& objSize, Vector3& otherPos, Vector3& otherSize)
{
	//Checks for collision between 2 objects and returns true if colliding
	return (std::abs(objPos.x - otherPos.x) * 2 < (objSize.x + otherSize.x)) &&
		(std::abs(objPos.y - otherPos.y) * 2 < (objSize.y + otherSize.y)) &&
		(std::abs(objPos.z - otherPos.z) * 2 < (objSize.z + otherSize.z));
}

//I'm keeping this function in the manager, but for some reason it cannot resolve collisions between the boxes, I don't really know why since
//it is basically identical to the already existing resolveCollision function, so I will continue to use the original function instead 
void PhysicsManager::ResolveOtherCollision(Vector3& objPos, Vector3& objVel, Vector3& otherPos, Vector3& otherVel)
{
	Vector3 normal = { objPos.x - otherPos.x, objPos.y - otherPos.y, objPos.z - otherPos.z };
	float length = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

	// Normalize the normal vector
	normal.normalise();

	float relativeVelocityX = objPos.x - otherPos.x;
	float relativeVelocityY = objPos.y - otherPos.y;
	float relativeVelocityZ = objPos.z - otherPos.z;

	// Compute the relative velocity along the normal
	float impulse = relativeVelocityX * normal.x + relativeVelocityY * normal.y + relativeVelocityZ * normal.z;

	// Ignore collision if objects are moving away from each other
	if (impulse > 0) {
		return;
	}

	// Compute the collision impulse scalar
	float e = 0.01f; // Coefficient of restitution (0 = inelastic, 1 = elastic)
	float dampening = 0.9f; // Dampening factor (0.9 = 10% energy reduction)
	float j = -(1.0f + e) * impulse * dampening;

	// Apply the impulse to the boxes' velocities
	objVel.x += j * normal.x;
	objVel.y += j * normal.y;
	objVel.z += j * normal.z;
	otherVel.x -= j * normal.x;
	otherVel.y -= j * normal.y;
	otherVel.z -= j * normal.z;
}

//Called in game loop to update deltaTime value
void PhysicsManager::UpdateDeltaTime()
{
	static auto last = std::chrono::steady_clock::now();
	auto old = last;
	last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = last - old;
	deltaTime = frameTime.count();
}

void* PhysicsManager::operator new(size_t size)
{
	//Overwritten class specific new so that all physics managers are tracked by tracker type physics
	return ::operator new(size, Tracker::Type::physics);
}