#include <math.h>

#include "vector3.h"

Vector3_t Vector3(float x, float y, float z) {
    Vector3_t V = (Vector3_t) {x, y, z};
    return V;
}

Vector3_t vec3_mul_scalar(const Vector3_t V, float T) {
    return Vector3(V.x * T, V.y * T, V.z * T);
}

Vector3_t vec3_add(const Vector3_t U, const Vector3_t V) {
    return Vector3(U.x + V.x, U.y + V.y, U.z + V.z);
}

Vector3_t vec3_sub(const Vector3_t U, const Vector3_t V) {
    return Vector3(U.x - V.x, U.y - V.y, U.z - V.z);
}

Vector3_t vec3_mul(const Vector3_t U, const Vector3_t V) {
    return Vector3(U.x * V.x, U.y * V.y, U.z * V.z);
}

float vec3_length(const Vector3_t V) {
    return sqrtf(V.x * V.x + V.y * V.y + V.z * V.z);
}

float vec3_dot(const Vector3_t U, const Vector3_t V) {
    return U.x * V.x + U.y * V.y + U.z * V.z;
}

Vector3_t vec3_normalize(const Vector3_t V) {
    float L = vec3_length(V);
    return Vector3(V.x / L, V.y / L, V.z / L);
}

