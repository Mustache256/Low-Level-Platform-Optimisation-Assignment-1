#include "Box.h"

void Box::genRandPos(Box* box)
{
  //Calculates random box position
    box->position.x = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f));
    box->position.y = 10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.0f));
    box->position.z = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 20.0f));
}

void Box::genRandVel(Box* box)
{
  //Calculates random box velocity
    float randomXVelocity = -1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));
    box->velocity = { randomXVelocity, 0.0f, 0.0f };
}

void Box::genRandCol(Box* box)
{
  //Calculates random box colour
    box->colour.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    box->colour.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    box->colour.z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

void Box::setBoxSize(Box* box, float x, float y, float z)
{
  //Sets box size
    box->size = { x, y, z };
}

void* Box::operator new(size_t size)
{
  //Calls overwritten operator new with tracker type box
    return ::operator new(size, Tracker::Type::box);
}


