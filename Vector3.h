#pragma once
#include <cmath>

class Vector3
{
public:
    float x, y, z;

    //Initialises zero vector
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    //Initialises vector with passes in values
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    ~Vector3() {};

    //Allows vectors to be subtracted from each other
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    // Normalize the vector
    void normalise();

    // get the length of a vector
    float length() const;
};

