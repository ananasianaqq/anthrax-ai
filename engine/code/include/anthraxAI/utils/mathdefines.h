#pragma once
#include <string>
#include <glm/glm.hpp>
template <typename T>
struct Vector2
{
    T x;
    T y;
};

template <typename T>
struct Vector3
{
    T x;
    T y;
    T z;
    
    Vector3() {}
    Vector3(T f) : x(f), y(f), z(f) {}
    Vector3(T f1, T f2, T f3) : x(f1), y(f2), z(f3) {}
    Vector3<T>& operator=(const glm::vec3& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        return *this;
    }
    Vector3<T>& operator=(const Vector3<T>& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        return *this;
    }
    glm::vec3 convert() { return glm::vec3(x, y, z); }
    std::string ToString() { return "x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", z: " + std::to_string(z); }
};
