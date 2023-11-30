#include "Vector3.h"

void Vector3::normalise()
{
  //Normalises the vector
    float length = std::sqrt(x * x + y * y + z * z);
    if (length != 0) {
        x /= length;
        y /= length;
        z /= length;
    }
}

float Vector3::length() const
{
  //Returns length of vector
    return std::sqrt(x * x + y * y + z * z);
}
